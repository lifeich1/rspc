#include "point2d.hh"

#include "common/assert.hh"

int main() {
    typedef A::Point2d<int> Pn;
    ASSERT_EQ(Pn(1, 1).mdis(Pn(2, 3)), 3);
    ASSERT_EQ(Pn(1, 1).mdis(Pn(3, 2)), 3);

    ASSERT_EQ(Pn(1, 1).near(Pn(3, 2)), false);
    ASSERT_EQ(Pn(1, 1).near(Pn(1, 2)), true);
    ASSERT_EQ(Pn(2, 2).near(Pn(1, 2)), true);

    ASSERT_EQ(std::less<Pn>{}(Pn(3, 1), Pn(1, 2)), false);
    ASSERT_EQ(std::less<Pn>{}(Pn(2, 2), Pn(1, 2)), false);
    ASSERT_EQ(std::less<Pn>{}(Pn(1, 2), Pn(1, 2)), false);
    ASSERT_EQ(std::less<Pn>{}(Pn(1, 1), Pn(1, 2)), true);
    ASSERT_EQ(std::less<Pn>{}(Pn(0, 3), Pn(1, 2)), true);

    ASSERT_EQ(Pn::YXLess{}(Pn(1, 3), Pn(2, 1)), false);
    ASSERT_EQ(Pn::YXLess{}(Pn(2, 2), Pn(2, 1)), false);
    ASSERT_EQ(Pn::YXLess{}(Pn(2, 1), Pn(2, 1)), false);
    ASSERT_EQ(Pn::YXLess{}(Pn(1, 1), Pn(2, 1)), true);
    ASSERT_EQ(Pn::YXLess{}(Pn(3, 0), Pn(2, 1)), true);

    {
        Pn p;
        std::stringstream{"11 22"} >> p;
        ASSERT_EQ(p.x, 11);
        ASSERT_EQ(p.y, 22);

        std::stringstream ss;
        ss << p;
        ASSERT_EQ(ss.str(), std::string{"11 22"});
    }
    return 0;
}
