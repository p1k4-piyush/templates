# Guide to Adding Custom Debugging & Graphviz Visualizations

This guide outlines the standard practices for extending the template repository with custom debugging hooks and automated Graphviz structural visualizations. By following these rules, you ensure that the debugging logic remains entirely isolated from production compilations, guaranteeing zero-overhead and strict compatibility with standard online judges.

## 1. The `GRACIE` Compilation Flag
All debugging utilities, formatting streams, and Graphviz generations are gated behind the `GRACIE` macro flag.

- **For Local Testing:** Compile with `g++ -DGRACIE your_file.cpp` to activate the debug features.
- **For Online Judges:** Omit the `-DGRACIE` flag. The preprocessor will actively strip out the entire `dbg.hpp` dependency and erase all debugging macros (`dbg()`, `dbgl()`), making them evaluate cleanly to nothing.

---

## 2. Implementing Basic Debug Print Methods

If you define a new data structure (like an `Info` or `Tag` struct) and want it to display cleanly when using `dbg(my_struct)`, you can use either a generic `print()` hook or standard `operator<<`. 

**Critical Rule:** You must wrap these formatting methods inside `#ifdef GRACIE ... #endif` to avoid dependencies like `<sstream>` bleeding into standard templates!

### Example: Using a `.print()` Hook
The `dbg` library automatically detects `.print()` using SFINAE. Use this for complex multi-value objects.

```cpp
struct SubarraySumInfo {
    long long sum, pref, suff, ans;
    // ... logic ...

#ifdef GRACIE
    std::string print() const {
        std::ostringstream os;
        os << "[" << sum << "|" << pref << "|" << suff << "|" << ans << "]";
        return os.str();
    }
#endif
};
```

### Example: Using `operator<<`
For simpler types that intuitively map to a standard output stream.

```cpp
struct Tag {
    long long add;
    // ... logic ...

#ifdef GRACIE
    friend std::ostream& operator<<(std::ostream& os, const Tag& tag) {
        if (tag.add != 0) os << "+" << tag.add;
        else os << "none";
        return os;
    }
#endif
};
```

---

## 3. Implementing Graphviz Visualizations

If you implement a tree, graph, or a complex algorithmic return type, you can instruct `dbg(...)` to automatically render the layout as a `.png` via Graphviz. Add a `std::string graphviz() const` method to your struct that returns a strictly valid Graphviz DOT string.

### Adding Graphviz to Existing Data Structures
If you are adding Graphviz to an existing structure that operates inherently offline natively (like `SegmentTree`), simply append the method wrapped in `#ifdef GRACIE`.

```cpp
template<typename Info>
class SegmentTree {
    // ... properties ...

#ifdef GRACIE
    std::string graphviz() const {
        std::ostringstream os;
        os << "digraph G {\n";
        // Create DOT representations mapped to your nodes via HTML labels or shapes...
        os << "}\n";
        return os.str();
    }
#endif
};
```

---

## 4. Complex Wrapper Results (Algorithms)

To generate detailed Graphviz states from complex algorithms without breaking the original return types, you must use **SFINAE Type Switching**. The algorithm should return a unique Wrapper struct when `GRACIE` is defined, but strictly alias natively down to the underlying `std::vector` when it is not.

### A. Define the Type Switch
Declare your visualization wrapper entirely wrapped in `#ifdef GRACIE`, and provide an exact `#else using ...` fallback. 

```cpp
#ifdef GRACIE
template <typename T>
struct my_algo_result : public std::vector<int> {
    const digraph<T>* g_ptr; // Debug tracing data
    
    std::string graphviz() const {
        // Output DOT using g_ptr and (*this) base states
        return "...";
    }
};
#else
// Fallback natively to the raw output type!
template <typename T>
using my_algo_result = std::vector<int>;
#endif
```

### B. Wrap Internal Property Assignments
Inside the actual algorithm function, wrap any state injection into the `g_ptr` (or other debugging properties) with `#ifdef GRACIE`! Failure to do so will cause a compilation error when the fallback natively drops those properties.

```cpp
template <typename T>
my_algo_result<T> solve_something(const digraph<T>& g) {
    my_algo_result<T> res;
    res.assign(g.n, 0);

#ifdef GRACIE
    res.g_ptr = &g; // Debug specific
#endif

    // Run original algorithm logic cleanly
    return res;
}
```

### Summary Checklist for Visualizers:
*   [ ] Does the struct contain a `.graphviz() const` returning a `.dot` layout string?
*   [ ] Is the ENTIRE wrapper struct enclosed in `#ifdef GRACIE`?
*   [ ] Is there an `#else` condition aliasing it to the native C++ return type?
*   [ ] Are all variable assignments tied to debug-only tracking variables properly wrapped in `#ifdef GRACIE` where the algorithm runs? 
*   [ ] Does the algorithm compile successfully with and without `g++ -DGRACIE`?
