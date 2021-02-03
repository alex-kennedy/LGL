#include "ed_lookup_table.h"

#include "sphere.hpp"

namespace lgl {
namespace lib {

template <typename Entry>
bool EDLookupTable<Entry>::closeToEntries(
    const EDLookupTable<Entry>::vec_type& v, FloatType cutoff) {
  // Go through each dimension and look for the one that
  // satisfies the cutoff
  for (typename vec_type::size_type ii = 0; ii < v.size(); ++ii) {
    DimensionEntries& dentries = table[ii];
    typename DimensionEntries::iterator theend = dentries.end();
    EDE lower;
    lower.value = v[ii] - cutoff;
    typename DimensionEntries::iterator begin = dentries.lower_bound(lower);
    EDE upper;
    upper.value = v[ii] + cutoff;
    typename DimensionEntries::iterator end = dentries.upper_bound(upper);
    if (begin != theend && end != theend) {
      for (; begin != end; ++begin) {
        FloatType d =
            euclideanDistance(begin->entry.location().begin(),
                              begin->entry.location().end(), v.begin());
        if (d < cutoff + begin->entry.radius()) {
          return true;
        }
      }
    }
  }
  return false;
}

template <typename Entry>
void EDLookupTable<Entry>::print(std::ostream& o) const {
  o << "ED LOOKUP TABLE\n";
  for (typename EDTable::size_type ii = 0; ii < table.size(); ++ii) {
    o << "\tDIMENSION: " << ii << '\n';
    for (typename DimensionEntries::const_iterator jj = table[ii].begin();
         jj != table[ii].end(); ++jj) {
      (jj)->print(o);
    }
  }
  o << "\tCURRENT ENTRIES:\n";
  for (typename EntryList::const_iterator ii = entries.begin();
       ii != entries.end(); ++ii) {
    o << *ii << '\n';
  }
}

template <typename Entry>
void EDLookupTable<Entry>::clear() {
  table.clear();
  entries.clear();
}

template <typename Entry>
bool EDLookupTable<Entry>::insert(Entry& e, vec_type& v) {
  if (entries.find(e.ID()) != entries.end()) {
    return false;
  }
  for (typename vec_type::size_type ii = 0; ii < v.size(); ++ii) {
    table[ii].insert(EDE(e, v[ii]));
  }
  entries.insert(e.ID());
  return true;
}

template <typename Entry>
EDLookupTable<Entry>& EDLookupTable<Entry>::operator=(
    const EDLookupTable<Entry>& t1) {
  table = t1.table;
  entries = t1.entries;
}

template class EDLookupTable<Sphere<FloatType>>;

}  // namespace lib
}  // namespace lgl
