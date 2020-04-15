#pragma once
#define IFRET(...) ({ if(auto x = __VA_ARGS__) return x; })

template<class P, class M>
size_t offsetof(const M P::*member) {
	return (size_t) &((P*)(0)->*member); }

template<class P, class M>
P* container_of(M* ptr, const M P::*member) {
	return (P*)( (char*)ptr - offsetof(member)); }
	
template<class P, class M, class S>
S* sibling_of(M* ptr, const M P::*member, const S P::*sib) {
	size_t diff = offsetof(sib)-offsetof(member);
	return (S*)( (char*)ptr + diff);  }


typedef int (*compar_t)(const void*,const void*);
#define COMPAR(x) ((compar_t)(void*)x)
