
//  https://github.com/the-tourist/algo/blob/master/segtree/tag.cpp

struct Tag {
    int add;

    Tag()
    {
        add = 0;
    }

    Tag(int val)
    {
        add = val;
    }

    bool ApplyTo(Info& a, [[maybe_unused]] int l, [[maybe_unused]] int r) const
    {

        return true;
    }

    void ApplyTo(Tag& t) const
    {

        t.add += add;
    }

    bool Empty() const
    {

        return add == 0;
    }
};
