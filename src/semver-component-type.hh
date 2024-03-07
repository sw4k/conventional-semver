// SPDX-License-Identifier: MIT
/*
 * semver-component-type.hh
 *
 * Copyright (C) sw4k
 * 
 * 
 */
#ifndef SEMVER_COMPONENT_TYPE_HH_
#define SEMVER_COMPONENT_TYPE_HH_

namespace csemver
{
    enum SemverComponentType
    {
        None = 0,
        Patch = 1,
        Minor = 2,
        Major = 3
    };
}

#endif