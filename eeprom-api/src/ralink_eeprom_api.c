
 /****************************************************************************
 *
 *  Copyright 2014, lintel<lintel.huang@gmail.com>  hoowa<hoowa.sun@gmail.com>
 *
 *                       谨以此中文警醒国人
 *    任何作品都有作者的辛苦劳动成果，虽然可以随意使用，但请保留作者的声明或者名字，这是一种激励。
 *    任何抹除作者信息的行为都是对作者开源本意的严重打击！
 *    自从出现XWiFi路由套用源码并彻底抹除作者信息之后，本人已心灰意冷。
 *    感谢众多OpenWrt的爱好者支持，鼓励并一起完成此驱动移植，与大家分享的喜悦,分享成果.
 *    
 *    本驱动的WiFi源码来自RT-N56U开源项目:https://code.google.com/p/rt-n56u/
 *    驱动版权归MTK/Ralink所有。
 *    This Driver base on MTK/Ralink RT2860v2 WiFi Driver.
 *    Thanks a lot of OpenWrt Project.
 * 
 *****************************************************************************/
 
 /* 
 * EEPROM Functions for Ralink/MTK Plantform v1.2
 * 
 * 
 * 
 * Flash API: ra_mtd_read, ra_mtd_write
 * Arguments:
 *   - num: specific the mtd number
 *   - to/from: the offset to read from or written to
 *   - len: length
 *   - buf: data to be read/written
 * Returns:
 *   - return -errno if failed
 *   - return the number of bytes read/written if successed
 */

#include "rt_config.h"

#include <linux/module.h>
#include <linux/types.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/version.h>
#include <linux/err.h>

#include <linux/slab.h>
#include <asm/io.h>
#include <linux/mtd/mtd.h>
#include <linux/mtd/map.h>
#include <linux/mtd/concat.h>
#include <linux/mtd/partitions.h>
#include <asm/addrspace.h>

#define BOOT_FROM_NOR	0
#define BOOT_FROM_NAND	2
#define BOOT_FROM_SPI	3

struct proc_dir_entry *procRegDir;

#if 0
int ra_check_flash_type(void)
{
#if defined (CONFIG_MTD_NOR_RALINK)
    return BOOT_FROM_NOR;
#elif defined (CONFIG_MTD_SPI_RALINK)
    return BOOT_FROM_SPI;
#elif defined (CONFIG_MTD_NAND_RALINK)
    return BOOT_FROM_NAND;
#else // CONFIG_MTD_ANY_RALINK //

    uint8_t Id[10];
    int syscfg=0;
    int boot_from=0;
    int chip_mode=0;

    memset(Id, 0, sizeof(Id));
    strncpy(Id, (char *)RALINK_SYSCTL_BASE, 6);
    syscfg = (*((int *)(RALINK_SYSCTL_BASE + 0x10)));

    if((strcmp(Id,"RT3052")==0) || (strcmp(Id, "RT3350")==0)) {
	boot_from = (syscfg >> 16) & 0x3; 
	switch(boot_from)
	{
	case 0:
	case 1:
	    boot_from=BOOT_FROM_NOR;
	    break;
	case 2:
	    boot_from=BOOT_FROM_NAND;
	    break;
	case 3:
	    boot_from=BOOT_FROM_SPI;
	    break;
	}
    }else if(strcmp(Id,"RT3883")==0) {
	boot_from = (syscfg >> 4) & 0x3; 
	switch(boot_from)
	{
	case 0:
	case 1:
	    boot_from=BOOT_FROM_NOR;
	    break;
	case 2:
	case 3:
	    chip_mode = syscfg & 0xF;
	    if((chip_mode==0) || (chip_mode==7)) {
		boot_from = BOOT_FROM_SPI;
	    }else if(chip_mode==8) {
		boot_from = BOOT_FROM_NAND;
	    }else {
		printk("unknow chip_mode=%d\n",chip_mode);
	    }
	    break;
	}
    }else if(strcmp(Id,"RT3352")==0) {
	boot_from = BOOT_FROM_SPI;
    }else if(strcmp(Id,"RT5350")==0) {
	boot_from = BOOT_FROM_SPI;
    }else if(strcmp(Id,"RT2880")==0) {
	boot_from = BOOT_FROM_NOR;
    }else if(strcmp(Id,"RT6855")==0) {
	boot_from = BOOT_FROM_SPI;
    }else if(strcmp(Id,"MT7620")==0) {
	chip_mode = syscfg & 0xF;
	switch(chip_mode)
	{
	case 0:
	case 2:
	case 3:
		boot_from = BOOT_FROM_SPI;
		break;
	case 1:
	case 10:
	case 11:
	case 12:
		boot_from = BOOT_FROM_NAND;
		break;	
	}	
    }else if(strcmp(Id,"MT7621")==0) {
	chip_mode = syscfg & 0xF;
	switch(chip_mode)
	{
	case 0:
	case 2:
	case 3:
		boot_from = BOOT_FROM_SPI;
		break;
	case 1:
	case 10:
	case 11:
	case 12:
		boot_from = BOOT_FROM_NAND;
		break;	
	}	
    } else {
	printk("%s: %s is not supported\n",__FUNCTION__, Id);
    }

    return boot_from;
#endif


}
#endif
int ra_mtd_write(int num, loff_t to, size_t len, const u_char *buf)
{
	int ret = -1;
	size_t rdlen, wrlen;
	struct mtd_info *mtd;
	struct erase_info ei;
	u_char *bak = NULL;

	mtd = get_mtd_device(NULL, num);
	if (IS_ERR(mtd))
		return (int)mtd;
	if (len > mtd->erasesize) {
		put_mtd_device(mtd);
		return -E2BIG;
	}

	bak = kmalloc(mtd->erasesize, GFP_KERNEL);
	if (bak == NULL) {
		put_mtd_device(mtd);
		return -ENOMEM;
	}
#if LINUX_VERSION_CODE < KERNEL_VERSION(3,4,1)
	ret = mtd->read(mtd, 0, mtd->erasesize, &rdlen, bak);
#else
	ret = mtd->_read(mtd, 0, mtd->erasesize, &rdlen, bak);
#endif
	if (ret != 0) {
		put_mtd_device(mtd);
		kfree(bak);
		return ret;
	}
	if (rdlen != mtd->erasesize)
		printk("warning: ra_mtd_write: rdlen is not equal to erasesize\n");

	memcpy(bak + to, buf, len);

	ei.mtd = mtd;
	ei.callback = NULL;
	ei.addr = 0;
	ei.len = mtd->erasesize;
	ei.priv = 0;
#if LINUX_VERSION_CODE < KERNEL_VERSION(3,4,1)
	ret = mtd->erase(mtd, &ei);
#else
	ret = mtd->_erase(mtd, &ei);
#endif
	if (ret != 0) {
		put_mtd_device(mtd);
		kfree(bak);
		return ret;
	}
#if LINUX_VERSION_CODE < KERNEL_VERSION(3,4,1)
	ret = mtd->write(mtd, 0, mtd->erasesize, &wrlen, bak);
#else
	ret = mtd->_write(mtd, 0, mtd->erasesize, &wrlen, bak);
#endif
	put_mtd_device(mtd);

	kfree(bak);
	return ret;
}


int ra_mtd_write_nm(char *name, loff_t to, size_t len, const u_char *buf)
{
  
	int ret = -1;
	size_t rdlen, wrlen;
	struct mtd_info *mtd;
	struct erase_info ei;
	u_char *bak = NULL;
	printk("EEPROM:write to [%s] offset 0x%x,length 0x%x.\n",name,(unsigned int )to,len);
	mtd = get_mtd_device_nm(name);
	if (IS_ERR(mtd))
		return (int)mtd;
	if (len > mtd->erasesize) {
		put_mtd_device(mtd);
		return -E2BIG;
	}

	bak = kmalloc(mtd->erasesize, GFP_KERNEL);
	if (bak == NULL) {
		put_mtd_device(mtd);
		return -ENOMEM;
	}
#if LINUX_VERSION_CODE < KERNEL_VERSION(3,4,1)
	ret = mtd->read(mtd, 0, mtd->erasesize, &rdlen, bak);
#else
	ret = mtd->_read(mtd, 0, mtd->erasesize, &rdlen, bak);
#endif
	if (ret != 0) {
		put_mtd_device(mtd);
		kfree(bak);
		return ret;
	}
	if (rdlen != mtd->erasesize)
		printk("warning: ra_mtd_write: rdlen is not equal to erasesize\n");

	memcpy(bak + to, buf, len);

	ei.mtd = mtd;
	ei.callback = NULL;
	ei.addr = 0;
	ei.len = mtd->erasesize;
	ei.priv = 0;
#if LINUX_VERSION_CODE < KERNEL_VERSION(3,4,1)
	ret = mtd->erase(mtd, &ei);
#else
	ret = mtd->_erase(mtd, &ei);
#endif
	if (ret != 0) {
		put_mtd_device(mtd);
		kfree(bak);
		return ret;
	}
#if LINUX_VERSION_CODE < KERNEL_VERSION(3,4,1)
	ret = mtd->write(mtd, 0, mtd->erasesize, &wrlen, bak);
#else
	ret = mtd->_write(mtd, 0, mtd->erasesize, &wrlen, bak);
#endif
	put_mtd_device(mtd);
	kfree(bak);
	return ret;
}

int ra_mtd_read(int num, loff_t from, size_t len, u_char *buf)
{
	int ret;
	size_t rdlen;
	struct mtd_info *mtd;

	mtd = get_mtd_device(NULL, num);
	if (IS_ERR(mtd))
		return (int)mtd;
#if LINUX_VERSION_CODE < KERNEL_VERSION(3,4,1)
	ret = mtd->read(mtd, from, len, &rdlen, buf);
#else
	ret = mtd->_read(mtd, from, len, &rdlen, buf);
#endif
	if (rdlen != len)
		printk("warning: ra_mtd_read: rdlen is not equal to len\n");

	put_mtd_device(mtd);
	return ret;
}


int ra_mtd_read_nm(char *name, loff_t from, size_t len, u_char *buf)
{
	int ret;
	size_t rdlen;
	struct mtd_info *mtd;
	printk("EEPROM:Read from [%s] offset 0x%x,length 0x%x.\n",name,(unsigned int )from,len);
	
	mtd = get_mtd_device_nm(name);
	if (IS_ERR(mtd))
		return (int)mtd;
#if LINUX_VERSION_CODE < KERNEL_VERSION(3,4,1)
	ret = mtd->read(mtd, from, len, &rdlen, buf);
#else
	ret = mtd->_read(mtd, from, len, &rdlen, buf);
#endif
	if (rdlen != len)
		printk("warning: ra_mtd_read_nm: rdlen is not equal to len\n");

	put_mtd_device(mtd);
	return ret;
}

int rt2860_irq_request(struct net_device *net_dev)
{
	net_dev->irq = 6;
	return 0;
}

int rt2860_eeprom_api_init(void)
{
  printk("Ralink/MTK WiFi EEPROM API Driver for OpenWrt. Copyright 2014 lintel<lintel.huang@gmail.com> \n");  
#if defined (CONFIG_RALINK_MT7620)
  printk("Ralink/MTK WiFi EEPROM API Build for MT7620. \n");
#elif (CONFIG_RALINK_RT3052)
  printk("Ralink/MTK WiFi EEPROM API Build for RT3052. \n");
#endif
  return 0;
}

void rt2860_eeprom_api_exit(void)
{
  printk("Ralink/MTK WiFi EEPROM API:Released. \n");
}

module_init(rt2860_eeprom_api_init);
module_exit(rt2860_eeprom_api_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("lintel <lintel.huang@gmail.com>");

EXPORT_SYMBOL(ra_mtd_write);
EXPORT_SYMBOL(ra_mtd_read);
EXPORT_SYMBOL(ra_mtd_write_nm);
EXPORT_SYMBOL(ra_mtd_read_nm);
EXPORT_SYMBOL(rt2860_irq_request);
EXPORT_SYMBOL(procRegDir);