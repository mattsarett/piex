// Copyright 2015 Google Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef PIEX_PIEX_TYPES_H_
#define PIEX_PIEX_TYPES_H_

#include <cstdint>
#include <string>

namespace piex {

// Defines the error codes used by piex.
enum Error {
  kOk,
  kFail,
  kUnsupported,
};

// Contains relevant image information as well as the 'jpeg_offset' and the
// 'jpeg_length' which are used to obtain the JPEG compressed preview image.
// 'full_width' and 'full_height' are correctly cropped but not rotated.
struct PreviewImageData {
  enum ColorSpace {
    kSrgb,
    kAdobeRgb,
  };
  struct Rational {
    std::uint32_t numerator = 0;
    std::uint32_t denominator = 1;
  };
  struct Gps {
    // Indicates if the gps data is valid to use.
    bool is_valid = false;

    char latitude_ref;  // Either 'N' or 'S'
    Rational latitude[3];
    char longitude_ref;  // Either 'E' or 'W'
    Rational longitude[3];
    bool altitude_ref = false;  // true is above, false below sea level
    Rational altitude;

    Rational time_stamp[3];  // Giving hour, minute and second.
    std::string date_stamp;  // Giving as "YYYY:MM:DD" format.
  };

  // Optional data to find the preview image and to handle it correctly.
  std::uint32_t jpeg_offset = 0;
  std::uint32_t jpeg_length = 0;
  std::uint32_t exif_orientation = 1;  // horizontal as default
  ColorSpace color_space = kSrgb;

  // Optional Exif metadata that describes the image.
  std::uint32_t full_width = 0;
  std::uint32_t full_height = 0;
  std::string maker;
  std::string model;
  std::string date_time;
  std::uint32_t iso = 0;
  Rational exposure_time;
  Rational fnumber;
  Rational focal_length;
  Gps gps;

  // Hint of the extracted preview size compared to the actual RAW image.
  // If full_preview == true, then the preview suppose to have a similar or
  // larger size then the RAW image, else the preview image might be
  // significantly smaller, e.g. only half of the RAW image size.
  bool full_preview;
};

// Defines the StreamInterface that needs to be implemented by the client.
class StreamInterface {
 public:
  virtual ~StreamInterface() {}

  // Reads 'length' amount of bytes from 'offset' to 'data'. The 'data' buffer
  // provided by the caller, guaranteed to be at least "length" bytes long.
  // On 'kOk' the 'data' pointer contains 'length' valid bytes beginning at
  // 'offset' bytes from the start of the stream.
  // Returns 'kFail' if 'offset' + 'length' exceeds the stream and does not
  // change the contents of 'data'.
  virtual Error GetData(const size_t offset, const size_t length,
                        std::uint8_t* data) = 0;
};

}  // namespace piex

#endif  // PIEX_PIEX_TYPES_H_
