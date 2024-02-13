#ifndef UTILS_H
#define UTILS_H

/// Returns the unsigned integer converted to a string.
/// @param num the unsigned integer to be converted
/// @return string with the unsigned integer
char* uint_to_string(unsigned int num);

/// Returns a new string with the extension of the given filename changed to the given extension.
/// @param filename original filename
/// @param new_extension the new extension
/// @return string with the new extension
char* filename_extension_changer(const char *filename, const char *new_extension);

#endif // UTILS_H