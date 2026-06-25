//////////////////////////////////////////////////////////////////////////////
/// \file regex.c
///
///
/// SECTIONS: Index of this file
///
/// [SECTION] Include Mess
/// [SECTION] Macro Defines
///
/// Internal Declarations
///
///     [SECTION] Internal : Forwards
///     [SECTION] Internal : Structures
///     [SECTION] Internal : Functions
///
/// Public Declarations
///
///     [SECTION] Public : Functions
///
//////////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------------
// [SECTION] Include Mess
//-----------------------------------------------------------------------------

#include <crack/regex.h>

//-----------------------------------------------------------------------------
// [SECTION] Macro Defines
//-----------------------------------------------------------------------------

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

#pragma region Private

    //-----------------------------------------------------------------------------
    // [SECTION] Internal : Forwards
    //-----------------------------------------------------------------------------

    //-----------------------------------------------------------------------------
    // [SECTION] Internal : Structures
    //-----------------------------------------------------------------------------

    //-----------------------------------------------------------------------------
    // [SECTION] Internal : Functions
    //-----------------------------------------------------------------------------

    //////////////////////////////////////////////////////////////////////////////
    /// Function Declarations
    //////////////////////////////////////////////////////////////////////////////

    //////////////////////////////////////////////////////////////////////////////
    /// Function Definations
    //////////////////////////////////////////////////////////////////////////////

#pragma endregion // Private

#pragma region Public

    //-----------------------------------------------------------------------------
    // [SECTION] Public : Functions
    //-----------------------------------------------------------------------------

    regex regex_init(const char *pattern) {};
    void regex_free(regex regex) {};

    match match_init() {};
    void match_free(match match) {};

    bool regex_match(char *string, match match, regex regex, flag flags) {};
    bool regex_search(char *string, match match, regex regex, flag flags) {};
    char *regex_replace(char *out, char *string, regex regex, char *format, flag flags) {};

#pragma endregion // Public

#ifdef __cplusplus
} // extern "C"
#endif // __cplusplus
