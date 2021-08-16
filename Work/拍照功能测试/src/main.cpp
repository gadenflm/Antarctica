#include "camera_kit.h"
// #include "recorder.h"

#include <algorithm>
#include <cstring>
#include <fcntl.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
using namespace std;
using namespace OHOS;
using namespace OHOS::Media;

// When camera is available or unavailable , Call back
// The class is from camera_device_callback.h
class WKL_CameraDeviceCallback : public CameraDeviceCallback {
    void OnCameraStatus(std::string cameraId, int32_t status) override {
        cout << "The Camera(id:" << cameraId << ") status changed." << '\n';
    }
};

//  Save a picture
static void WKL_save_capture(const char *p, uint32_t size) {
    cout << "************************ Saving Capture ************************\n";
    struct timeval tv = {};
    gettimeofday(&tv, nullptr);
    struct tm *ltm = localtime(&tv.tv_sec);
    if (ltm != nullptr) {
        ostringstream ss("Capture_");
        ss << "Capture" << ltm->tm_hour << "-" << ltm->tm_min << "-" << ltm->tm_sec << ".jpg";
        ofstream pic("/nfs/data/" + ss.str(), ofstream::out | ofstream::trunc);
        pic.write(p, size);
        pic.close();
        cout << "write " << size << " bytes" << '\n';
        cout << "************************ Saving Finished ************************" << endl;
    }

}


// when frame finished,
class WKL_FrameStateCallback : public FrameStateCallback {
    void OnFrameFinished(Camera &camera, FrameConfig &fc, FrameResult &result) override
    {
        cout << "************************ Frame Finished ************************" << endl;

        if (fc.GetFrameConfigType() == FRAME_CONFIG_CAPTURE) {
            cout << "[Wkl Info]\tCapture frame received.\n";

            // not change:
            list<Surface *> surface_list = fc.GetSurfaces();
            for (Surface *surface : surface_list) {
                SurfaceBuffer *buffer = surface->AcquireBuffer();
                if (buffer != nullptr) {
                    char *virt_addr = static_cast<char *>(buffer->GetVirAddr());
                    if (virt_addr != nullptr) {
                        WKL_save_capture(virt_addr, buffer->GetSize());
                    }
                    surface->ReleaseBuffer(buffer);
                }
                delete surface;
            }
        }
        delete &fc;
    }
};


// The class is from camera_state_callback.h
class WKL_CameraStateMng : public CameraStateCallback {
public:
    // constructor or release
    WKL_CameraStateMng() = delete;
    WKL_CameraStateMng(EventHandler &eventHdlr) : eventHdlr_(eventHdlr) {}
    ~WKL_CameraStateMng() {
        // CloseRecorder();
    }

    void OnCreated(Camera &c) override {
        cout << "************************ Camera Created ************************\n";
        cout << "Camera.GetCameraId() return:" << c.GetCameraId() << '\n';

        auto cam_cfg =  CameraConfig::CreateCameraConfig();
        cam_cfg->SetFrameStateCallback(&wkl_fscb_ , &eventHdlr_);
        c.Configure(*cam_cfg);
        wkl_cam_ = &c;
    }
    void OnCreateFailed(const std::string cameraId, int32_t errorCode) override {
        cout << "[Wkl Error]\tCamera created failed with errorCode:" <<  errorCode << endl;
    }
    void OnReleased(Camera &c) override {
        cout << "************************ Camera Released ************************" << endl;
    }

    void Capture() {
        if (wkl_cam_ == nullptr) {
            cout << "[Wkl Error]\tCamera is not ready." << endl;
            return;
        }
        FrameConfig *wkl_fc = new FrameConfig(FRAME_CONFIG_CAPTURE);
        Surface *cam_surface = Surface::CreateSurface();

        if (cam_surface == nullptr) {
            delete wkl_fc;
            cout << "[Wkl Error]\tCreateSurface failed" << endl;
            return;
        }
        cam_surface->SetWidthAndHeight(1920, 1080);
        wkl_fc->AddSurface(*cam_surface);
        cout << "************************ Trigger a Capture ************************\n";
        // One Capture:
        wkl_cam_->TriggerSingleCapture(*wkl_fc);

    }

private:
    /*    enum State : int32_t { STATE_IDLE, STATE_RUNNING, STATE_BUTT };
        State previewState_ = STATE_IDLE;
        State recordState_ = STATE_IDLE;*/

    EventHandler &eventHdlr_;
    Camera *wkl_cam_ = nullptr;

    // int32_t recordFd_ = -1;
    // Recorder *recorder_ = nullptr;
    WKL_FrameStateCallback wkl_fscb_;
};

int main()
{
    cout << "[Wkl Info] Media Test Begin" << endl;

    cout << "************************ Camera Init ************************\n";
    // 1.Get CameraKit
    CameraKit *cam_kit = CameraKit::GetInstance();
    if (cam_kit == nullptr) {
        cout << "[Wkl Error]\tCan not get CameraKit instance!" << endl;
        return 0;
    }

    // 2.Get CameraAbility
    list<string> cam_list = cam_kit->GetCameraIds();
    string cam_id;

    int index = 0;
    for (auto &cam : cam_list) {
        cout << "cam_list[" << index++ << "]<string>: " << cam << '\n';
        const CameraAbility* cam_ability = cam_kit->GetCameraAbility(cam);

        list<CameraPicSize> size_list = cam_ability->GetSupportedSizes(0);

        for  (auto &pic_size : size_list) {
            printf ("Width: %u\t\tHeight: %u\n", pic_size.width, pic_size.height);
        }
        cam_id = cam;
    }

    // CreateCamera:
    EventHandler event_handler; // Create a thread to handle callback events
    WKL_CameraStateMng cam_state_mng(event_handler);

    cam_kit->CreateCamera(cam_id, cam_state_mng, event_handler);

    getchar();
    getchar();

    cam_state_mng.Capture();

    cout << "[Wkl Info] Media Test End" << endl;
    return 0;
}
