/*
 *
 *  Copyright (c) 2002 Alex Adai, All Rights Reserved.
 *
 *  This program is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License as
 *  published by the Free Software Foundation; either version 2 of
 *  the License, or (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 *  MA 02111-1307 USA
 *
 */
#ifndef LGL_LIB_TIME_KEEPER_H_
#define LGL_LIB_TIME_KEEPER_H_

#include <iostream>

#include "types.h"

namespace lgl {
namespace lib {

class TimeKeeper {
 private:
  void init_vars() {
    time_step_ = 0;
    iteration_ = 0;
    total_time_ = 0;
    max_iteration_ = 0;
    min_iteration_ = 0;
  }

 protected:
  FloatType time_step_;
  FloatType total_time_;
  unsigned int iteration_;
  unsigned int max_iteration_;
  unsigned int min_iteration_;

 public:
  TimeKeeper() { TimeKeeper::init_vars(); }

  void increment(FloatType dt) {
    ++iteration_;
    total_time_ += dt;
  }

  void increment() {
    ++iteration_;
    total_time_ += time_step_;
  }

  unsigned int iteration() const { return iteration_; }
  FloatType time_step() const { return time_step_; }
  void time_step(FloatType dt) { time_step_ = dt; }
  FloatType time() const { return total_time_; }
  void max(unsigned int t) { max_iteration_ = t; }
  FloatType max() const { return max_iteration_; }
  void min(FloatType t) { min_iteration_ = t; }
  FloatType min() const { return min_iteration_; }

  bool rangeCheck() const {
    return ((iteration_ >= min_iteration_) && (iteration_ <= max_iteration_));
  }

  void copy(const TimeKeeper& tk) {
    total_time_ = tk.total_time_;
    iteration_ = tk.iteration_;
    time_step_ = tk.time_step_;
    min_iteration_ = tk.min_iteration_;
    max_iteration_ = tk.max_iteration_;
  }

  void operator=(const TimeKeeper& tk) { TimeKeeper::copy(tk); }

  void operator++() { TimeKeeper::increment(); }

  void operator+=(FloatType dt) { TimeKeeper::increment(dt); }
};

}  // namespace lib
}  // namespace lgl

#endif  // LGL_LIB_TIME_KEEPER_H_
