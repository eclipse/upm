%include "autobox.i"

%{
#include <vector>
#include <stdexcept>
%}

%include <stdint.i>
%include <std_except.i>

namespace std {

	template<class T> class vector {
	public:
        typedef size_t size_type;
        typedef T value_type;
        typedef const value_type& const_reference;
        vector();
        vector(size_type n);
        vector(const vector& o);
        size_type capacity() const;
        void reserve(size_type n);
        %rename(isEmpty) empty;
        bool empty() const;
        void clear();
        void push_back(const value_type& x);
        %extend {
            const_reference get_impl(int i) throw (std::out_of_range) {
                // at will throw if needed, swig will handle
                return self->at(i);
            }
			value_type set(int i, const value_type& VECTOR_VALUE_IN) throw (std::out_of_range) {
                const T old = $self->at(i);
                $self->at(i) = VECTOR_VALUE_IN;
                return old;
            }
            int32_t size() const {
              return $self->size();
            }
            void removeRange(int32_t from, int32_t to) {
              $self->erase($self->begin()+from, $self->begin()+to);
            }
        }
	};
}

%typemap(javabase) std::vector "java.util.AbstractList<$typemap(autobox,$1_basetype::value_type)>"
%typemap(javainterface) std::vector "java.util.RandomAccess"
%typemap(jstype) std::vector set "$typemap(autobox,$1_basetype)"
%typemap(jstype) std::vector &VECTOR_VALUE_IN "$typemap(autobox,$1_basetype)"
%typemap(javacode) std::vector %{
	$javaclassname(java.util.Collection<$typemap(autobox,$1_basetype::value_type)> e) {
		this.reserve(e.size());
	    for($typemap(autobox,$1_basetype::value_type) value : e) {
			this.push_back(value);
	    }
	}
	public $typemap(autobox,$1_basetype::value_type) get(int idx) {
		return get_impl(idx);
	}
%}