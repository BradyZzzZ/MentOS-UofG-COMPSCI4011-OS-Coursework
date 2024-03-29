/// @file isr.h
/// @brief Data structures concerning the Interrupt Service Routines (ISRs).
/// @copyright (c) 2014-2024 This file is distributed under the MIT License.
/// See LICENSE.md for details.
/// @addtogroup descriptor_tables Descriptor Tables
/// @{
/// @addtogroup isr Interrupt Service Routines (ISRs)
/// @brief are software routines that hardware invokes in response to an interrupt.
/// @{

#pragma once

#include "kernel.h"

// TODO: see interrupt_handler_t in Linux, it is quite different.
/// @brief      Interrupt handler definition.
/// @details    An interrupt handler is an interrupt service routine called
///             to manage interrupt requests, or CPU execptions.
/// @param f    An interrupt stack frame.
typedef void (*interrupt_handler_t)(pt_regs *f);

/// @brief For each exceptions isrs_init sets a default handler which
///        prints the rose exceptions and stops kernel execution.
void isrs_init(void);

/// @brief For each interrupt irq_init sets a default handler which
///        prints the rose IRQ line and stops kernel execution.
void irq_init(void);

/* Even if an interrupt service routine is called for exceptions and
 * interrupts, we use two distinct methods to logically setup an ISR to
 * handle theme. Tecnically speacking, exceptions are synchronous interrupts
 * generated by CPU. For an exception we have an ISR. Interrupts are
 * asynchronous events generated by PIC. Furthermore, multiple ISRs can be set
 * for a same IRQ.
 */

/// @brief Installs an ISR to handle an interrupt.
/// @param i interrupt identifier.
/// @param handler interrupt handler.
/// @param description interrupt description.
/// @return 0 on success, -1 otherwise.
int isr_install_handler(unsigned i, interrupt_handler_t handler, char *description);

/// @brief Uninstall an ISR handler.
/// @param i interrupt identifier.
/// @return 0 on success, -1 otherwise.
int isr_uninstall_handler(unsigned i);

/// @brief Installs an ISR to handle an interrupt.
/// @param i interrupt identifier.
/// @param handler interrupt handler.
/// @param description interrupt description.
/// @return 0 on success, -1 otherwise.
int irq_install_handler(unsigned i, interrupt_handler_t handler, char *description);

/// @brief Uninstall an IRQ handler.
/// @param i interrupt identifier.
/// @param handler interrupt handler.
/// @return 0 on success, -1 otherwise.
int irq_uninstall_handler(unsigned i, interrupt_handler_t handler);

/// @brief Method called by CPU to handle interrupts.
/// @param f The interrupt stack frame.
extern void irq_handler(pt_regs *f);

/// @brief Method called by CPU to handle exceptions.
/// @param f The interrupt stack frame.
extern void isq_handler(pt_regs *f);

//==== List of exceptions generated internally by the CPU ======================
#define DIVIDE_ERROR        0  ///< DE Divide Error.
#define DEBUG_EXC           1  ///< DB Debug.
#define NMI_INTERRUPT       2  ///< Non Mascable Interrupt.
#define BREAKPOINT          3  ///< BP Breakpoint.
#define OVERFLOW            4  ///< OF Overflow.
#define BOUND_RANGE_EXCEED  5  ///< BR Bound Range Exception.
#define INVALID_OPCODE      6  ///< UD Invalid OpCode Exception.
#define DEV_NOT_AVL         7  ///< NM Device Not Available.
#define DOUBLE_FAULT        8  ///< DF Double Fault.
#define COPROC_SEG_OVERRUN  9  ///< Coprocessor Segment Overrun.
#define INVALID_TSS         10 ///< TS Invalid TSS.
#define SEGMENT_NOT_PRESENT 11 ///< NP Segment Not Present.
#define STACK_SEGMENT_FAULT 12 ///< SS Stack Segment Fault.
#define GENERAL_PROTECTION  13 ///< GP General Protection.
#define PAGE_FAULT          14 ///< PF Page Fault.
#define INT_RSV             15 ///< XX Reserverd.
#define FLOATING_POINT_ERR  16 ///< MF Floating Point.
#define ALIGNMENT_CHECK     17 ///< AC Alignment Check.
#define MACHINE_CHECK       18 ///< MC Machine Check.
#define SIMD_FP_EXC         19 ///< XF Streaming SIMD Exception.
#define VIRT_EXC            20 ///< Virtualization Exception.
// Reserved [21-29].
#define SECURITY_EXC 30 ///< Security Exception.
#define TRIPLE_FAULT 31 ///< Triple Fault
#define SYSTEM_CALL  80 ///< System call interrupt.
                        //==============================================================================

/// @}
/// @}
