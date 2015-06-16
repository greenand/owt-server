/*
 * Copyright 2015 Intel Corporation All Rights Reserved. 
 * 
 * The source code contained or described herein and all documents related to the 
 * source code ("Material") are owned by Intel Corporation or its suppliers or 
 * licensors. Title to the Material remains with Intel Corporation or its suppliers 
 * and licensors. The Material contains trade secrets and proprietary and 
 * confidential information of Intel or its suppliers and licensors. The Material 
 * is protected by worldwide copyright and trade secret laws and treaty provisions. 
 * No part of the Material may be used, copied, reproduced, modified, published, 
 * uploaded, posted, transmitted, distributed, or disclosed in any way without 
 * Intel's prior express written permission.
 * 
 * No license under any patent, copyright, trade secret or other intellectual 
 * property right is granted to or conferred upon you by disclosure or delivery of 
 * the Materials, either expressly, by implication, inducement, estoppel or 
 * otherwise. Any license under such intellectual property rights must be express 
 * and approved by Intel in writing.
 */

#ifndef AudioExternalOutput_h
#define AudioExternalOutput_h

#include <JobTimer.h>
#include <MediaMuxing.h>
#include <webrtc/modules/audio_device/include/fake_audio_device.h>
#include <WebRTCTransport.h>

namespace mcu {

class AudioExternalOutput : public erizo::MediaSink, public woogeen_base::MediaMuxing, public woogeen_base::JobTimerListener {
public:
    AudioExternalOutput();
    virtual ~AudioExternalOutput();

    // Implements the MediaSink interfaces.
    virtual int deliverAudioData(char* buf, int len);
    virtual int deliverVideoData(char* buf, int len);

    // Implements the MediaMuxing interfaces.
    virtual int32_t startMuxing(const std::string& participant, int codec, woogeen_base::MediaFrameQueue& audioQueue);
    virtual void stopMuxing(int32_t id);
    virtual bool getVideoSize(unsigned int& width, unsigned int& height) const;

    // Implements the JobTimerListener interface.
    void onTimeout();

private:
    void init();
    int32_t addOutput(int codec, woogeen_base::MediaFrameQueue& audioQueue);
    void removeOutput(int32_t channelId);

    webrtc::VoiceEngine* m_voiceEngine;
    boost::scoped_ptr<webrtc::FakeAudioDeviceModule> m_adm;

    int32_t m_inputChannelId;
    std::map<int32_t, boost::shared_ptr<woogeen_base::AudioEncodedFrameCallbackAdapter>> m_outputTransports;
    boost::shared_mutex m_outputMutex;

    boost::scoped_ptr<woogeen_base::JobTimer> m_jobTimer;
};

} /* namespace mcu */

#endif /* AudioExternalOutput_h */