#ifndef DECL_EXPORT_H
#define DECL_EXPORT_H

#ifndef DECL_EXPORT
#	if defined(_MSC_VER)
#		define DECL_EXPORT __declspec(dllexport)
#	elif defined(__GNUC__)
#		define DECL_EXPORT __attribute__((visibility("default")))
#	else
#		error   "Unknown Operation System!"
#	endif
#endif

#ifndef DECL_IMPORT
#	if defined(_MSC_VER)
#		define DECL_IMPORT __declspec(dllimport)
#	else
#		define DECL_IMPORT
#	endif
#endif

#endif