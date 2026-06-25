//////////////////////////////////////////////////////////////////////////////
/// \file <crack/regex.h>
///
//////////////////////////////////////////////////////////////////////////////

#ifndef CRACK_REGEX_H

//////////////////////////////////////////////////////////////////////////////
///
/// SECTIONS: Index of this file
///
/// [SECTION] Include Mess
/// [SECTION] Macro Definitions
///
/// Public Declarations
///
///     [SECTION] Public : Forwards
///     [SECTION] Public : Structures
///     [SECTION] Public : Functions
///
/// [SECTION] Internal Cleanups
///
//////////////////////////////////////////////////////////////////////////////

#define CRACK_REGEX_H

//-----------------------------------------------------------------------------
// [SECTION] Public Declarations
//-----------------------------------------------------------------------------

//////////////////////////////////////////////////////////////////////////////
/// \defgroup regex Regular Expressions
/// \ingroup text
///
/// A facility for performing regular expression pattern matching.
///
/// \{
//////////////////////////////////////////////////////////////////////////////

#pragma region Public

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

    //-----------------------------------------------------------------------------
    // [SECTION] Public : Forwards
    //-----------------------------------------------------------------------------

    //-----------------------------------------------------------------------------
    // [SECTION] Public : Structures
    //-----------------------------------------------------------------------------

    typedef enum flag
    {
        data,
    } flag;

    //////////////////////////////////////////////////////////////////////////////
    /// \brief A regular expression
    ///
    //////////////////////////////////////////////////////////////////////////////
    typedef struct regex
    {
        /* data */
    } *regex;

    //////////////////////////////////////////////////////////////////////////////
    /// \brief The results of a match or search operation.
    //////////////////////////////////////////////////////////////////////////////
    typedef struct match
    {
        char **results;
        char *(*group)(int);

    } *match;

    //-----------------------------------------------------------------------------
    // [SECTION] Public : Functions
    //-----------------------------------------------------------------------------

    //////////////////////////////////////////////////////////////////////////////
    /// \name Regex Construction, Copying, and Destruction
    /// \{
    //////////////////////////////////////////////////////////////////////////////

    //////////////////////////////////////////////////////////////////////////////
    /// \brief Constructs a regular expression from the string
    ///
    /// \param string A string containing a regular expression.
    /// \param flags Flags indicating the syntax rules and options.
    ///
    /// \returns regex The constructed regular expression.
    //////////////////////////////////////////////////////////////////////////////
    regex regex_init(const char *pattern);

    //////////////////////////////////////////////////////////////////////////////
    /// \brief Destroys a regular expression.
    //////////////////////////////////////////////////////////////////////////////
    void regex_free(regex regex);

    /// \} Regex Construction, Copying, and Destruction

    //////////////////////////////////////////////////////////////////////////////
    /// \name Match Construction, Copying, and Destruction
    /// \{
    //////////////////////////////////////////////////////////////////////////////

    //////////////////////////////////////////////////////////////////////////////
    /// \brief Constructs a match results.
    ///
    /// \returns regex The constructed match results.
    //////////////////////////////////////////////////////////////////////////////
    match match_init();

    //////////////////////////////////////////////////////////////////////////////
    /// \brief Destroys a match results.
    //////////////////////////////////////////////////////////////////////////////
    void match_free(match match);

    /// \} Match Construction, Copying, and Destruction

    //////////////////////////////////////////////////////////////////////////////
    /// \name Matching, Searching, and Replacing
    /// \{
    //////////////////////////////////////////////////////////////////////////////

    //////////////////////////////////////////////////////////////////////////////
    /// \brief Determines if there is a match between a regex and a string.
    ///
    /// \param string The string to match.
    /// \param match The match results.
    /// \param regex The regular expression.
    /// \param flags Controls how the regular expression is matched.
    ///
    /// \retval true  A match exists.
    /// \retval false Otherwise.
    //////////////////////////////////////////////////////////////////////////////
    bool regex_match(char *string, match match, regex regex, flag flags);

    //////////////////////////////////////////////////////////////////////////////
    /// \brief Searches for a regular expression within a string.
    ///
    /// \param string The string to search for the regex.
    /// \param match The match results.
    /// \param regex The regular expression to search.
    /// \param flags Search policy flags.
    ///
    /// \retval true  A match was found within the string.
    /// \retval false No match was found within the string.
    //////////////////////////////////////////////////////////////////////////////
    bool regex_search(char *string, match match, regex regex, flag flags);

    //////////////////////////////////////////////////////////////////////////////
    /// \brief Search for a regular expression within a range for multiple times,
    /// and replace the matched parts through filling a format string.
    ///
    /// \param out The output to the string after replacing.
    /// \param string The string to search and replace.
    /// \param regex The regular expression to search.
    /// \param format The format string.
    /// \param flags Search and replace policy flags.
    ///
    /// \returns out The string after replacing.
    //////////////////////////////////////////////////////////////////////////////
    char *regex_replace(char *out, char *string, regex regex, char *format, flag flags);

    /// \} Matching, Searching, and Replacing

#ifdef __cplusplus
} // extern "C"
#endif // __cplusplus

#pragma endregion // Public

/// \} group regex

//-----------------------------------------------------------------------------
// [SECTION] Internal Cleanups
//-----------------------------------------------------------------------------

#endif // CRACK_REGEX_H
