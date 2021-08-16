/**
 * bld/cmd_tftp.c
 *
 * Author: Anthony Ginger <hfjiang@ambarella.com>
 *
 * Copyright (c) 2016 Ambarella International LP
 *
 * This file and its contents ("Software") are protected by intellectual
 * property rights including, without limitation, U.S. and/or foreign
 * copyrights. This Software is also the confidential and proprietary
 * information of Ambarella International LP and its licensors. You may not use, reproduce,
 * disclose, distribute, modify, or otherwise prepare derivative works of this
 * Software or any portion thereof except pursuant to a signed license agreement
 * or nondisclosure agreement with Ambarella International LP or its authorized affiliates.
 * In the absence of such an agreement, you agree to promptly notify and return
 * this Software to Ambarella International LP
 *
 * THIS SOFTWARE IS PROVIDED "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 * INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF NON-INFRINGEMENT,
 * MERCHANTABILITY, AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL AMBARELLA INTERNATIONAL LP OR ITS AFFILIATES BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; COMPUTER FAILURE OR MALFUNCTION; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 */

#include <amboot.h>
#include <bldfunc.h>
#include <eth/network.h>
#include <sdmmc.h>

/*===========================================================================*/
static int cmd_tftp(int argc, char *argv[])
{
	int ret_val = 0;
	char cmdline[MAX_CMDLINE_LEN];

	if (argc < 2) {
		return -1;
	}

	if (strcmp(argv[1], "boot") == 0) {
		if (argc > 2) {
			strfromargv(cmdline, sizeof(cmdline),
			            (argc - 2), &argv[2]);
			bld_netboot(cmdline, 1);
		} else {
			bld_netboot(NULL, 1);
		}
	} else if (strcmp(argv[1], "program") == 0) {
		const char *pfile_name = "bld_release.bin";
		u32 addr = MEMFWPROG_RAM_START;
		u32 exec = 1;

		if (argc >= 5) {
			pfile_name = (const char *)argv[2];
			strtou32(argv[3], &addr, NULL);
			strtou32(argv[4], &exec, NULL);
		} else if (argc >= 4) {
			pfile_name = (const char *)argv[2];
			strtou32(argv[3], &addr, NULL);
		} else if (argc >= 3) {
			pfile_name = (const char *)argv[2];
		}
		bld_netprogram(pfile_name, addr, exec, 1);
	}
	else if (strcmp(argv[1], "write") == 0)
	{
		// tvt add 2021-01-07
		const char *pfile_name = "bld_release.bin";
		const char* pszFlashType = "bld";
		u32 u32FlashSize = 0;
		u32 u32SaveAddr = MEMFWPROG_RAM_START;
		if (argc >= 5)
		{
			pfile_name = (const char *)argv[2];
			pszFlashType = (const char *)argv[3];
			strtou32(argv[4], &u32FlashSize, NULL);
			// 先清空内存区
			printf("clear mem area\n");
			memset((void*)u32SaveAddr, 0, u32FlashSize);
			// tftp 获取
			printf("recv file[%s]\n", pfile_name);
			ret_val = bld_netprogram(pfile_name, u32SaveAddr, 0, 1);
			if (ret_val == 0)
			{
				if (strcmp(pszFlashType, "all") == 0)
				{
					// 烧写flash_all
					/// TODO:未实现
					printf("flash all...\n");
					return ret_val;
				}
				// 写入emmc
				printf("program %s into mmc, flash size[%u]\n", pszFlashType, u32FlashSize);
				sdmmc_write_partition(pszFlashType, (u8*)u32SaveAddr, u32FlashSize, 1);
				printf("\n");
			}
		}
		else
		{
			printf("err command\n");
			ret_val = -2;
		}
	}
	else {
		ret_val = -2;
	}

	return ret_val;
}

static char help_tftp[] =
    "tftp boot [cmdline]\n"
    "tftp program [file_name] [addr] [exec]\n"
    "tftp write [file_name] [flash type] [flash size]\n"
    "Load images from TFTP server\n";
__CMDLIST(cmd_tftp, "tftp", help_tftp);

