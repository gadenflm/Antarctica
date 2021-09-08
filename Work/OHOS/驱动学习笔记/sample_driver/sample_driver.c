#include "hdf_device_desc.h" //HDF header
#include "hdf_log.h" // HDF log header

#define HDF_LOG_TAG sample_driver   // The default log tag

struct WklDebugDriverService
{	
	struct IDeviceIoService ioService;

	//Out inteface of driver service
	
};

// Driver bind to HDF 
int32_t HdfSampleDriverBind ( struct HdfDeviceObject* deviceObject){
	if (deviceObject == NULL){
		HDF_LOGE("Sample device object is null");
		return -1;
	}

	static struct WklDebugDriverService sampleDriver;

	deviceObject->service = &sample_driver.ioService;

	HDF_LOGE("Sample driver bind success!");
	return 0;
}

// Init
int32_t HdfSampleDriverInit (struct HdfDeviceObject* deviceObject){
	HDF_LOGE("Sample driver init success!");
	return 0;
}

// Release
void HdfSampleDriverRelease ( struct HdfDeviceObject* deviceObject){
	HDF_LOGE("Sample driver release success!");
	return;
}

//Define the Object of Driver Entry !!

struct HdfDriverEntry g_sampleDriverEntry = {
	.moduleVersion = 1,
	.moduleName = "sample_driver",
	.Bind = HdfSampleDriverBind,
	.Init = HdfSampleDriverInit,
	.Release = HdfSampleDriverRelease,
};

// HDF Driver register !! The Driver will be loaded with this order: 1.Bind 2.Release

HDF_INIT (g_sampleDriverEntry);

