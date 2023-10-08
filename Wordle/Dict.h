#pragma once

#include "PCH.h"

namespace Dict {
    void load();
    bool isExist(const QString &word);
    QString generate();
}
