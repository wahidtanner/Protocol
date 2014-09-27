//
//  TokenReader.cpp
//  Protocol
//
//  Created by Wahid Tanner on 9/11/14.
//

#include "TokenReader.h"

using namespace std;
using namespace MuddledManaged;

const string Protocol::TokenReader::TokenIterator::Whitespace = " \t\r\n";
const string Protocol::TokenReader::TokenIterator::Delimiters = "{}[]=;\"";

Protocol::TokenReader::TokenIterator::TokenIterator ()
: mpData(nullptr)
{
}

Protocol::TokenReader::TokenIterator::TokenIterator (Protocol::TokenReader::TokenReaderData * pData)
: mpData(pData)
{
    // Advance to the first token right away.
    moveNext();
}

Protocol::TokenReader::TokenIterator::TokenIterator (const Protocol::TokenReader::TokenIterator & src)
: mpData(src.mpData)
{
}

Protocol::TokenReader::TokenIterator & Protocol::TokenReader::TokenIterator::operator = (const Protocol::TokenReader::TokenIterator & rhs)
{
    if (this == &rhs)
    {
        return *this;
    }

    mpData = rhs.mpData;

    return *this;
}

bool Protocol::TokenReader::TokenIterator::operator == (const TokenIterator & rhs) const
{
    return !this->operator!=(rhs);
}

bool Protocol::TokenReader::TokenIterator::operator != (const TokenIterator & rhs) const
{
    if (!atEnd() || !rhs.atEnd())
    {
        return true;
    }
    return false;
}

void Protocol::TokenReader::TokenIterator::moveNext ()
{
    if (atEnd())
    {
        return;
    }

    if (mpData->mDelimiter != '\0')
    {
        // When we find a delimiter, that means we have
        // already read it from the stream. We just set this delimiter
        // and return on the subsequent moveNext() call
        mpData->mCurrentToken = mpData->mDelimiter;
        mpData->mDelimiter = '\0';
        return;
    }

    string text = "";
    bool previousForwardSlash = false;
    bool previousStar = false;
    bool previousEscape = false;
    bool ignoreToEndOfLine = false;
    bool ignoreToEndOfComment = false;
    bool returningString = false;
    char c;
    while (mpData->mProtoStream->get(c))
    {
        if (mpData->mStringMode)
        {
            // An opening quote was found for the last token. That
            // begins string processing for this token until the end quote is
            // found or the end of the line. Strings are not allowed to span
            // lines, so if we find a newline before the end quote, then return
            // what we have.
            if (c == '\n')
            {
                // It's possible that an opening quote could appear at the end
                // of the line. This is the end of the string even if it is still
                // empty.
                mpData->mStringMode = false;
                returningString = true;
                break;
            }

            if (c == '\\')
            {
                if (previousEscape)
                {
                    text += "\\\\";
                    previousEscape = false;
                }
                else
                {
                    previousEscape = true;
                }
                continue;
            }

            if (c != '\"' || previousEscape)
            {
                if (previousEscape)
                {
                    text += '\\';
                    previousEscape = false;
                }
                text += c;
                continue;
            }

            // It's possible that an ending quote could appear right after
            // the opening quote. This is the end of the string regardless of
            // whether or not there was any text inside the string.
            mpData->mStringMode = false;
            mpData->mDelimiter = '\"';
            returningString = true;
            break;
        }

        if (ignoreToEndOfComment)
        {
            if (c == '/' && previousStar)
            {
                previousStar = false;
                ignoreToEndOfComment = false;
            }
            else if (c == '*')
            {
                previousStar = true;
            }
            else
            {
                previousStar = false;
            }
            continue;
        }

        if (ignoreToEndOfLine)
        {
            if (c == '\n')
            {
                ignoreToEndOfLine = false;

                if (!text.empty())
                {
                    // Treat the end of the line like whitespace if we already
                    // have text.
                    break;
                }
            }
            continue;
        }

        string::size_type index = Delimiters.find(c);
        if (index == string::npos)
        {
            index = Whitespace.find(c);
            if (index == string::npos)
            {
                if (c == '/')
                {
                    if (previousForwardSlash)
                    {
                        // This is a single-line comment. There could still
                        // be text on the same line before the comment.
                        previousForwardSlash = false;
                        ignoreToEndOfLine = true;
                        continue;
                    }
                    previousForwardSlash = true;
                    continue;
                }
                else
                {
                    if (c == '*' && previousForwardSlash)
                    {
                        // This is a potential multi-line comment. There could still
                        // be text on the same line before or after the comment.
                        previousForwardSlash = false;
                        ignoreToEndOfComment = true;
                        continue;
                    }

                    if (previousForwardSlash)
                    {
                        text += '/';
                    }
                    text += c;
                }
            }
            else
            {
                if (previousForwardSlash)
                {
                    // This is a single forward slash followed by whitespace. There
                    // may or may not have been other text or whitespace before the
                    // forward slash.
                    text += '/';
                    previousForwardSlash = false;
                }

                if (!text.empty())
                {
                    break;
                }
            }
        }
        else
        {
            if (c == '\"')
            {
                // This will cause the next token after the opening quote
                // to be processed as a string.
                mpData->mStringMode = true;
            }

            if (previousForwardSlash)
            {
                text += '/';
            }

            mpData->mDelimiter = c;
            break;
        }
    }

    if (previousEscape)
    {
        // Just in case there is a single escape char inside a string
        // that is located at the end of the line or the end of the file.
        text += '\\';
    }

    if (returningString || !text.empty())
    {
        // Return what was found. This could be nothing if it is
        // an empty string.
        mpData->mCurrentToken = text;
    }
    else
    {
        if (mpData->mDelimiter != '\0')
        {
            // There was no text found but we did find a delimiter.
            // Set the delimiter as the current token.
            mpData->mCurrentToken = mpData->mDelimiter;
            mpData->mDelimiter = '\0';
        }
        else
        {
            // If there was nothing found, then we must be at the end.
            mpData->mCurrentToken = "";
            mpData->mEnd = true;
        }
    }
}

bool Protocol::TokenReader::TokenIterator::atEnd () const
{
    if (mpData == nullptr || mpData->mEnd)
    {
        return true;
    }

    return false;
}

Protocol::TokenReader::TokenIterator & Protocol::TokenReader::TokenIterator::operator ++ ()
{
    moveNext();

    return *this;
}

std::string Protocol::TokenReader::TokenIterator::operator * () const
{
    if (mpData == nullptr)
    {
        return "";
    }
    
    return mpData->mCurrentToken;
}

const std::string * Protocol::TokenReader::TokenIterator::operator -> () const
{
    if (mpData == nullptr)
    {
        return nullptr;
    }

    return &mpData->mCurrentToken;
}

Protocol::TokenReader::TokenReaderData::TokenReaderData (ifstream * protoStream)
: mEnd(false), mStringMode(false), mDelimiter('\0'), mCurrentToken(""), mProtoStream(protoStream)
{
}

void Protocol::TokenReader::TokenReaderData::reset ()
{
    mEnd = false;
    mStringMode = false;
    mDelimiter = '\0';
    mCurrentToken = "";
    mProtoStream->clear();
    mProtoStream->seekg(0);
}

Protocol::TokenReader::TokenReader (const std::string & protoFileName)
: mData(new Protocol::TokenReader::TokenReaderData(new ifstream(protoFileName)))
{
}

Protocol::TokenReader::~TokenReader ()
{
}

Protocol::TokenReader::iterator Protocol::TokenReader::begin ()
{
    mData->reset();
    
    return iterator(mData.get());
}

Protocol::TokenReader::iterator Protocol::TokenReader::end ()
{
    return iterator();
}

