
#ifndef LGL_LIB_ED_LOOKUP_TABLE_HPP_
#define LGL_LIB_ED_LOOKUP_TABLE_HPP_

#include <set>
#include <vector>

#include "cube.h"
#include "fixedVecOperations.hpp"

namespace lgl {
namespace lib {

template <typename Entry>
struct EDPlace {
  Entry entry;
  FloatType value;
  EDPlace() {}
  EDPlace(Entry& er, FloatType vv) : entry(er), value(vv) {}
  void print(std::ostream& o = std::cout) const {
    o << "ENTRY: ";
    entry.print();
    o << "VALUE: " << value << '\n';
  }
  bool operator<(const EDPlace<Entry>& e2) const { return value < e2.value; }
  bool operator==(const EDPlace<Entry>& e2) const { return value == e2.value; }
};

template <typename Entry>
class EDLookupTable {
 public:
  typedef Entry value_type;
  typedef typename std::vector<FloatType> vec_type;
  typedef typename std::set<std::string> EntryList;
  typedef typename std::vector<value_type> ResultList;

 private:
  typedef EDLookupTable<value_type> LookupTable;
  typedef EDPlace<value_type> EDE;
  typedef typename std::multiset<EDE> DimensionEntries;
  typedef typename std::vector<DimensionEntries> EDTable;
  typedef typename DimensionEntries::const_iterator const_dentry_iterator;

  EDLookupTable() {}

 protected:
  EDTable table;
  EntryList entries;

 public:
  // CONSTRUCTORS
  explicit EDLookupTable(typename EDTable::size_type dimension)
      : table(dimension) {}

  // ACCESSORS
  bool closeToEntries(const vec_type& v, FloatType cutoff);

  void print(std::ostream& o = std::cout) const;

  // MUTATORS
  void clear();

  bool insert(Entry& e, vec_type& v);

  // OPERATORS
  LookupTable& operator=(const LookupTable& t1);

  ~EDLookupTable() {}
};

}  // namespace lib
}  // namespace lgl

#endif  // LGL_LIB_ED_LOOKUP_TABLE_HPP_
