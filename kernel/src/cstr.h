#pragma once
#include <stddef.h>
#include <stdint.h>

const char* tostr(uint64_t val); //Unsigned Integer to ASCII

const char* tostr(int64_t val); //Signed Integer to ASCII

const char* tostr(double val, uint8_t dec_places); //Double to ASCII

const char* tostr(double val); //Double to ASCII

const char* h_tostr(uint64_t value); //Hex to ASCII

const char* h_tostr(uint32_t value); //Hex to ASCII

const char* h_tostr(uint16_t value); //Hex to ASCII

const char* h_tostr(uint8_t value); //Hex to ASCII