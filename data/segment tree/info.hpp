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

#ifdef GRACIE
    std::string print() const
    {
        std::ostringstream os;
        _dbglib::dbg_impl(os, cur, false);
        return os.str();
    }
#endif
};
