// Copyright 2012 Google Inc. All Rights Reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "syzygy/agent/asan/stack_capture_cache.h"

#include "gtest/gtest.h"

namespace agent {
namespace asan {

TEST(StackCaptureTest, InitFromBuffer) {
  StackCapture capture;

  // Validate the capture's initial state.
  EXPECT_FALSE(capture.IsValid());
  EXPECT_EQ(0, capture.num_frames());
  EXPECT_EQ(NULL, capture.frames());

  // Create some fake stack trace data.
  ULONG stack_id = 0;
  void* frames[StackCapture::kMaxNumFrames + 1] = { 0 };
  for (size_t i = 0; i < arraysize(frames); ++i) {
    frames[i] = reinterpret_cast<void*>(i);
  }

  // Initialize the stack capture without using all of the frames.
  capture.InitFromBuffer(frames, 7);
  EXPECT_TRUE(capture.IsValid());
  EXPECT_EQ(7, capture.num_frames());
  EXPECT_TRUE(capture.frames() != NULL);

  // Attempt to initialize the stack capture using too many frames; the
  // resulting capture should truncate to kMaxNumFrames.
  capture.InitFromBuffer(frames, arraysize(frames));
  EXPECT_TRUE(capture.IsValid());
  EXPECT_EQ(StackCapture::kMaxNumFrames, capture.num_frames());
  EXPECT_TRUE(capture.frames() != NULL);
}

TEST(StackCaptureCacheTest, SaveStackTrace) {
  StackCaptureCache cache;

  // Capture a stack trace.
  ULONG stack_id = 0;
  void* frames[StackCapture::kMaxNumFrames] = { 0 };
  size_t num_frames = ::CaptureStackBackTrace(
      0, StackCapture::kMaxNumFrames, frames, &stack_id);

  // We should be able to save the captures stack trace.
  const StackCapture* s1 = cache.SaveStackTrace(stack_id, frames, num_frames);
  ASSERT_TRUE(s1 != NULL);

  // We should get a pointer to the initial stack capture object if we attempt
  // to save the trace again.
  const StackCapture* s2 = cache.SaveStackTrace(stack_id, frames, num_frames);
  EXPECT_EQ(s1, s2);

  // Capture a new stack trace.
  num_frames = ::CaptureStackBackTrace(
      0, StackCapture::kMaxNumFrames, frames, &stack_id);

  // We should get a pointer to a new stack capture object when we attempt
  // to save a different trace.
  const StackCapture* s3 = cache.SaveStackTrace(stack_id, frames, num_frames);
  EXPECT_NE(s1, s3);
}

}  // namespace asan
}  // namespace agent
