# Copyright 2019 The MediaPipe Authors.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

licenses(["notice"])

package(default_visibility = ["//mediapipe/examples:__subpackages__"])

cc_library(
    name = "hs_pipeline",
    srcs = ["pipeline.cc"],
    hdrs = ["pipeline.h"],
    deps = [],
    visibility = ["//visibility:public"],
    alwayslink = 1
)

cc_library(
    name = "json_lib",
    srcs = ["json.hpp"],
    visibility = ["//visibility:public"],
    alwayslink = 1
)

cc_binary(
    name = "demo",
    srcs = ["demo.cc"],
    deps = [
        "//mediapipe/framework:calculator_framework",
        "//mediapipe/framework/formats:image_frame",
        "//mediapipe/framework/formats:image_frame_opencv",
        "//mediapipe/framework/formats:landmark_cc_proto",
        "//mediapipe/framework/port:commandlineflags",
        "//mediapipe/framework/port:file_helpers",
        "//mediapipe/framework/port:opencv_highgui",
        "//mediapipe/framework/port:opencv_imgproc",
        "//mediapipe/framework/port:opencv_video",
        "//mediapipe/framework/port:parse_text_proto",
        "//mediapipe/framework/port:status",
        "//mediapipe/graphs/hs_solution:face_tracking",
        "//mediapipe/examples/desktop/demo:json_lib",
        "//mediapipe/examples/desktop/demo:hs_pipeline"
    ],
)
