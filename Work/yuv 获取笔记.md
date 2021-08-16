
内存设置相关 API
* IAV_IOC_QUERY_MEMBLOCK
* IAV_IOC_FREE_MEM_PART

数据获取相关 API
* IAV_IOC_QUERY_DESC

#思路整理 (获取YUV)

##初始化：先尝试不改初始化过程，从现有的main buffer 中抓取

思路：

ioctl(iav的fd,IAV_IOC_QUERY_MEMBLOCK,参数)
获得canvas0在iav文件中的地址。
然后mmap 建立本地申明的虚拟地址与 canvas0的映射关系
最后 读取canvas0，观察数据格式。



__`cavans_id=0`__

申请内存，参考:
```C
struct iav_querymem stQueryMem;
    struct iav_mem_part_info* pstPartInfo;
    stQueryMem.mid = IAV_MEM_PARTITION;
    pstPartInfo = &stQueryMem.arg.partition;
    pstPartInfo->pid = IAV_PART_BSB;
    AM_IOCTL(s32IavFD, IAV_IOC_QUERY_MEMBLOCK, &stQueryMem);
    if (pstPartInfo->mem.length == 0) 
    {
        TIPC_PRINTF_ERR("IAV_PART_BSB is not allocated.");
        return -1;
    }
```

建立上述内存映射:
```C 
    g_stVencCfg.u32BsbSize = pstPartInfo->mem.length;
    g_stVencCfg.pszBsbMem = mmap(NULL, g_stVencCfg.u32BsbSize * 2, PROT_READ, MAP_SHARED, s32IavFD, pstPartInfo->mem.addr);
    if (g_stVencCfg.pszBsbMem == MAP_FAILED) 
    {
        perror("mmap (%d) failed: %s\n");
        return -1;
    }

    TIPC_PRINTF_INFO("bsb_mem = %p, size = 0x%x\n", g_stVencCfg.pszBsbMem, g_stVencCfg.u32BsbSize);
```

