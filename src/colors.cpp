#include "colors.h"

const Color darkPurple = {31, 31, 88, 255};
const Color green = {47, 230, 23, 255};
const Color red = {232, 18, 18, 255};
const Color orange = {226, 116, 17, 255};
const Color yellow = {237, 234, 4, 255};
const Color purple = {166, 0, 247, 255};
const Color cyan = {21, 204, 209, 255};
const Color blue = {13, 64, 216, 255};
const Color lightPurple = {89, 79, 126, 255};
const Color lighterPurple = {142, 130, 177, 255};

std::vector<Color> GetCellColors() {
    return {darkPurple, green, red, orange, yellow, purple, cyan, blue};
}