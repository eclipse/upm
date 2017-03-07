/* Standardized vector types usable in SWIG for those C++ functions
 * that use them.
 */

%include "std_vector.i"
%include "stdint.i"

namespace std {
  %template(intVector) vector<int>;
  %template(int16Vector) vector<int16_t>;
  %template(floatVector) vector<float>;
  %template(doubleVector) vector<double>;
  %template(byteVector) vector<uint8_t>;
}
