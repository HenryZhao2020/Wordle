#include "Color.h"
#include "Attr.h"

QString rgb(Color color) {
    if (color == Color::GREEN) {
        if (Attr::colorBlind) {
            return "245, 121, 58";
        }
        return "82, 142, 82";
    }

    if (color == Color::YELLOW) {
        if (Attr::colorBlind) {
            return "133, 192, 249";
        }
        return "181, 160, 66";
    }

    return "168, 168, 168";
}
