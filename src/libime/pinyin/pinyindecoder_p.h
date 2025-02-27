/*
 * Copyright (C) 2017~2017 by CSSlayer
 * wengxt@gmail.com
 *
 * This library is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation; either version 2.1 of the
 * License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; see the file COPYING. If not,
 * see <http://www.gnu.org/licenses/>.
 */
#ifndef _FCITX_LIBIME_PINYIN_PINYINDECODER_P_H_
#define _FCITX_LIBIME_PINYIN_PINYINDECODER_P_H_

#include "pinyindecoder.h"
#include <libime/core/lattice.h>

namespace libime {

class PinyinLatticeNodePrivate : public LatticeNodeData {
public:
    PinyinLatticeNodePrivate(boost::string_view encodedPinyin)
        : encodedPinyin_(encodedPinyin.to_string()) {}

    std::string encodedPinyin_;
};
} // namespace libime

#endif // _FCITX_LIBIME_PINYIN_PINYINDECODER_P_H_
