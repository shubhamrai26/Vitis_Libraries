/*
 * (c) Copyright 2020 Xilinx, Inc. All rights reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

/**
 * @file zstd_decompress_stream.cpp
 * @brief Kernel source for zstd decompression streaming kernel.
 *
 * This file is part of Vitis Data Compression Library.
 */

#include "zstd_decompress_stream.hpp"

constexpr int getDataPortWidth(int maxVal) {
    return (31 - __builtin_clz(maxVal));
}

extern "C" {
void xilZstdDecompressStream(uint64_t input_size,
                             hls::stream<ap_axiu<c_streamDWidth, 0, 0, 0> >& inaxistreamd,
                             hls::stream<ap_axiu<c_streamDWidth, 0, 0, 0> >& outaxistreamd,
                             hls::stream<ap_axiu<64, 0, 0, 0> >& sizestreamd) {
#pragma HLS INTERFACE s_axilite port = input_size bundle = control
#pragma HLS interface axis port = inaxistreamd
#pragma HLS interface axis port = outaxistreamd
#pragma HLS interface axis port = sizestreamd
#pragma HLS INTERFACE s_axilite port = return bundle = control
    // data width for literal length, match length and offset data
    const int c_lmoDWidth = 1 + getDataPortWidth(c_windowSize);

    // Call for decompression
    xf::compression::zstdDecompressCore<MULTIPLE_BYTES, ZSTD_BLOCK_SIZE_KB, c_windowSize, c_lmoDWidth>(
        inaxistreamd, outaxistreamd, sizestreamd, input_size);
}
}
