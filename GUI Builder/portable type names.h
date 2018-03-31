#ifndef PORTABLE_TYPE_NAMES_H
#define PORTABLE_TYPE_NAMES_H

#include <string>
#include <typeinfo>
#include <type_traits>

// TODO: find a better solution...
// TODO: remove typeid and handle '[]' arrays. Handle more cases
// TODO: add tests!
template<typename Type>
inline std::string name()
{
	using namespace std;

	if(is_array<Type>::value)
		return name<typename remove_extent<Type>::type>() + '[' + to_string(extent<Type,rank<Type>::value==0?0:rank<Type>::value-1>::value) + ']';
	if(is_const<Type>::value)
		return name<typename remove_const<Type>::type>() + " const";
	if(is_volatile<Type>::value)
		return name<typename remove_volatile<Type>::type>() + " volatile";
	if(is_lvalue_reference<Type>::value)
		return name<typename remove_reference<Type>::type>() + " &";
	if(is_rvalue_reference<Type>::value)
		return name<typename remove_reference<Type>::type>() + " &&";
	if(is_pointer<Type>::value)
		return name<typename remove_pointer<Type>::type>() + " *";


	if(is_void<Type>::value) return "void";
	if(is_same<Type,nullptr_t>::value) return "nullptr_t";
	if(is_same<Type,bool>::value) return "bool";
	if(is_same<Type,char>::value) return "char";
	if(is_same<Type,wchar_t>::value) return "wchar_t";
	if(is_same<Type,char16_t>::value) return "char16_t";
	if(is_same<Type,char32_t>::value) return "char32_t";
	if(is_same<Type,unsigned char>::value) return "unsigned char";
	if(is_same<Type,signed char>::value) return "signed char";
	if(is_same<Type,unsigned short>::value) return "unsigned short";
	if(is_same<Type,signed short>::value) return "signed short";
	if(is_same<Type,unsigned int>::value) return "unsigned int";
	if(is_same<Type,signed int>::value) return "signed int";
	if(is_same<Type,unsigned long>::value) return "unsigned long";
	if(is_same<Type,signed long>::value) return "signed long";
	if(is_same<Type,unsigned long long>::value) return "unsigned long long";
	if(is_same<Type,signed long long>::value) return "signed long long";
	if(is_same<Type,float>::value) return "float";
	if(is_same<Type,double>::value) return "double";
	if(is_same<Type,long double>::value) return "long double";

	return typeid(Type).name();
} // end function name

#endif
