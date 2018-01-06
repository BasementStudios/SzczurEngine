#pragma once

#ifndef NDEBUG
#   define DEBUG
#   define DEBUGING if(true)
#   define ASSERT(...) assert(__VA_ARGS_)
#elif
#   undef DEBUG
#   define DEBUGING if(false)
#   define ASSERT(...) 
#endif
