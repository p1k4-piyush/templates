
//  https://github.com/the-tourist/algo/blob/master/segtree/info.cpp

struct Info {
    int cur;

    Info()
    {
        cur = 0;
    }

    Info(int val)
    {
        cur = val;
    }

    Info Unite(const Info& b) const
    {
        Info res;

        return res;
    }

    static Info GetDefault([[maybe_unused]] int l, [[maybe_unused]] int r)
    {
        return Info();
    }
};
