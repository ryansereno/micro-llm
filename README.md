# Micro-LLM: Running LLM Operations on an ATmega328P

An experimental implementation of LLM matrix operations on a $2 microcontroller with SD card memory offloading.

![ATmega328P with SD Card Setup](https://ryansereno.com/assets/Signal%20Note%20to%20Self%20-%202024-09-05%2008_15_12.Dko9Mq5f.jpeg)

## Overview
This project explores the minimum hardware requirements for running LLM operations by implementing matrix multiplication and self-attention calculations on an ATmega328P microcontroller (2KB RAM, 32KB Flash). While running a full LLM is infeasible, the project successfully demonstrates core mathematical operations using SD card memory offloading.

## Features
- Basic integer matrix multiplication
- SD card memory offloading for large matrices
- Serial communication for validation
- Quantized 8-bit integer operations
- Implementation of Q,K,V attention calculations

## Components
- Test validation kernels
- Arduino implementation code
- SD card memory management
- Matrix multiplication routines
- Serial communication utilities

## Requirements
- ATmega328P microcontroller
- SD card module
- Arduino IDE/CLI
- Python (for validation scripts)

## Limitations
- 2KB RAM constraint
- No floating-point hardware
- Sequential processing only
- Limited by SD card read/write speeds

## Read More
[Full Project Write-up](https://ryansereno.com/micro-llm.html)

## License
MIT
