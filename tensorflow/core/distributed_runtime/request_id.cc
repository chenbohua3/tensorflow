/* Copyright 2017 The TensorFlow Authors. All Rights Reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
==============================================================================*/

#include "tensorflow/core/distributed_runtime/request_id.h"

#include "tensorflow/core/platform/types.h"

namespace tensorflow {

int64_t GetUniqueRequestId() {
  int64_t request_id = 0;
  while (request_id == 0) {
    request_id = random::New64();
  }
  return request_id;
}

ShardUniqueRequestIdGenerator::ShardUniqueRequestIdGenerator(
    uint64_t num_shards, uint64_t shard_id)
    : generator_(random::RandomGenerator::kUrandom),
      num_shards_(num_shards),
      shard_id_(shard_id) {
  mask_ = 1;
  while (mask_ < num_shards_) {
    mask_ <<= 1;
  }
  mask_ = ~(mask_ - 1);
}

int64_t ShardUniqueRequestIdGenerator::GetUniqueRequestId() {
  int64_t request_id = 0;
  while (request_id == 0) {
    request_id = (generator_.New64() & mask_) | shard_id_;
  }
  return request_id;
}

}  // namespace tensorflow
