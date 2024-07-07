/**
 * The MIT License (MIT)
 *
 * Copyright (c) 2022 Peter Burgess
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NON INFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE
 */

#pragma once


#include "concepts/subscriber_concepts.h"

#include <optional>
#include <type_traits>


namespace dispatch {


template<typename RAW_RETURN_TYPE>
struct _GetRequestReturnType_ {
    using RETURN_TYPE =
            std::conditional_t<std::is_reference<RAW_RETURN_TYPE>::value, std::nullptr_t,
                    std::conditional_t<_is_non_value_request_return_type_t_<RAW_RETURN_TYPE>::value, RAW_RETURN_TYPE, std::optional<RAW_RETURN_TYPE>>>;
};

template<class RAW_RETURN_TYPE, class RETURN_TYPE = typename _GetRequestReturnType_<RAW_RETURN_TYPE>::RETURN_TYPE>
struct Request;

template<class RETURN_TYPE>
struct Request<RETURN_TYPE, RETURN_TYPE> {
    using _RETURN_TYPE_ = RETURN_TYPE;
};

template<class RETURN_TYPE>
struct Request<RETURN_TYPE, std::optional<RETURN_TYPE>> {
    using _RETURN_TYPE_ = std::optional<RETURN_TYPE>;
};


} // namespace dispatch
