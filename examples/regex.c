#include <crack/format.h>
#include <crack/print.h>
#include <crack/regex.h>

int main()
{
    // Sample target strings
    char *exact_email = "user@example.com";
    char *text_block = "Contact us at support@company.com or sales@company.org for help.";

    // 1. Raw string literal pattern matching an email address
    // R"()" prevents having to use double backslashes like "\\w+"
    regex email_pattern = regex_init("(\\w+@\\w+\\.\\w+)");

    //////////////////////////////////////////////////////////////////////////////
    /// 1. regex_match (Matches ENTIRE string)
    //////////////////////////////////////////////////////////////////////////////

    if (regex_match(exact_email, NULL, email_pattern, 0))
    {
        print("1. regex_match: '{}' is a valid email.\n", exact_email);
    }

    // This will fail because regex_match doesn't look for partial substrings
    if (!regex_match(text_block, NULL, email_pattern, 0))
    {
        print("1. regex_match: Failed on text_block (does not match from start to end).\n");
    }

    //////////////////////////////////////////////////////////////////////////////
    /// 2. regex_search (Finds SUBSTRINGS & Captures)
    //////////////////////////////////////////////////////////////////////////////

    match match; // Stores the matching pieces

    // We update the regex to use capturing groups () to extract pieces
    regex capture_pattern = regex_init("((\\w+)@(\\w+)\\.(\\w+))");

    if (regex_search(text_block, match, capture_pattern, 0))
    {
        print("\n2. regex_search: Found a match in the text block!\n");
        print("   Full match: {}\n", match->results[0]); // Group 0 is always the full match
        print("   Username:   {}\n", match->results[1]); // Group 1 is the first ()
        print("   Domain:     {}\n", match->results[2]); // Group 2 is the second ()
        print("   Extension:  {}\n", match->results[3]); // Group 3 is the third ()
        print("   Extension:  {}\n", match->group(3));   // Group 3 is the third ()
    }

    //////////////////////////////////////////////////////////////////////////////
    /// 3. regex_replace (Modifies Text)
    //////////////////////////////////////////////////////////////////////////////

    // Replace all email domains with 'hidden.com'
    char *censored_text = regex_replace(NULL, text_block, capture_pattern, "REDACTED@hidden.com", 0);
    print("\n3. regex_replace output:\n{}\n", censored_text);

    return 0;
}

//////////////////////////////////////////////////////////////////////////////
/// LICENSE: Public Domain (www.unlicense.org)
///
/// Copyright (c) 2026 Sackey Ezekiel Etrue
///
/// This is free and unencumbered software released into the public domain.
/// Anyone is free to copy, modify, publish, use, compile, sell, or distribute this
/// software, either in source code form or as a compiled binary, for any purpose,
/// commercial or non-commercial, and by any means.
///
/// In jurisdictions that recognize copyright laws, the author or authors of this
/// software dedicate any and all copyright interest in the software to the public
/// domain. We make this dedication for the benefit of the public at large and to
/// the detriment of our heirs and successors. We intend this dedication to be an
/// overt act of relinquishment in perpetuity of all present and future rights to
/// this software under copyright law.
///
/// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
/// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
/// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
/// AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
/// ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
/// WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
///
/// For more information, please refer to <http://unlicense.org/>
//////////////////////////////////////////////////////////////////////////////
