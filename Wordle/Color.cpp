#include "Color.h"
#include "Attr.h"

// Base style sheet with the 'r', 'g', 'b', 'a' values unspecified
const QString baseSheet = "color: white; background: rgba(%0, %1, %2, %3)";

QString rgba(const Color &color, double alpha) {
    // Whether color blind mode is enabled
    bool colorBlind = Attr::get().colorBlind;

    switch (color) {
    case Color::GREEN:
        return colorBlind ? baseSheet.arg(245).arg(121).arg(58).arg(alpha)
                          : baseSheet.arg(82).arg(142).arg(82).arg(alpha);
    case Color::YELLOW:
        return colorBlind ? baseSheet.arg(133).arg(192).arg(249).arg(alpha)
                          : baseSheet.arg(181).arg(160).arg(66).arg(alpha);
    default:
        return baseSheet.arg(128).arg(128).arg(128).arg(alpha);
    }
}
