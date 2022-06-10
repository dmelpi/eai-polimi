/**************************************************************************/
/*                                                                        */
/*       Copyright (c) Microsoft Corporation. All rights reserved.        */
/*                                                                        */
/*       This software is licensed under the Microsoft Software License   */
/*       Terms for Microsoft Azure RTOS. Full text of the license can be  */
/*       found in the LICENSE file at https://aka.ms/AzureRTOS_EULA       */
/*       and in the root directory of this software.                      */
/*                                                                        */
/**************************************************************************/


/**************************************************************************/
/**************************************************************************/
/**                                                                       */ 
/** USBX Component                                                        */ 
/**                                                                       */
/**   Asix Class                                                          */
/**                                                                       */
/**************************************************************************/
/**************************************************************************/


/* Include necessary system files.  */

#define UX_SOURCE_CODE

#include "ux_api.h"
#include "ux_host_class_asix.h"
#include "ux_host_stack.h"


/**************************************************************************/ 
/*                                                                        */ 
/*  FUNCTION                                               RELEASE        */ 
/*                                                                        */ 
/*    _ux_host_class_asix_transmission_callback           PORTABLE C      */ 
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Chaoqiong Xiao, Microsoft Corporation                               */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */ 
/*    This function is the callback from the USBX transfer functions,     */ 
/*    it is called when a full or partial transfer has been done for a    */ 
/*    bulk out transfer.                                                  */
/*                                                                        */
/*  INPUT                                                                 */ 
/*                                                                        */ 
/*    transfer_request                      Pointer to transfer request   */ 
/*                                                                        */ 
/*  OUTPUT                                                                */ 
/*                                                                        */ 
/*    None                                                                */ 
/*                                                                        */ 
/*  CALLS                                                                 */ 
/*                                                                        */ 
/*    _ux_host_stack_transfer_request       Process transfer request      */ 
/*    _ux_utility_short_put                 Put 16-bit value              */
/*    nx_packet_transmit_release            Release NetX packet           */
/*                                                                        */ 
/*  CALLED BY                                                             */ 
/*                                                                        */ 
/*    Application                                                         */ 
/*                                                                        */ 
/*  RELEASE HISTORY                                                       */ 
/*                                                                        */ 
/*    DATE              NAME                      DESCRIPTION             */ 
/*                                                                        */ 
/*  05-19-2020     Chaoqiong Xiao           Initial Version 6.0           */
/*  09-30-2020     Chaoqiong Xiao           Modified comment(s),          */
/*                                            resulting in version 6.1    */
/*                                                                        */
/**************************************************************************/
VOID  _ux_host_class_asix_transmission_callback (UX_TRANSFER *transfer_request)
{

UX_HOST_CLASS_ASIX              *asix;
NX_PACKET                       *current_packet;
NX_PACKET                       *next_packet;
UCHAR                           *packet_header;
    
    /* Get the class instance for this transfer request.  */
    asix =  (UX_HOST_CLASS_ASIX *) transfer_request -> ux_transfer_request_class_instance;
    
    /* Check the state of the transfer.  If there is an error, we do not proceed with this report.  */
    if (transfer_request -> ux_transfer_request_completion_code != UX_SUCCESS)
    {
        
        /* We do not proceed.  */
        return;        
        
    }

    /* Check if the class is in shutdown.  */
    if (asix -> ux_host_class_asix_state ==  UX_HOST_CLASS_INSTANCE_SHUTDOWN)

        /* We do not proceed.  */
        return;

    /* Check if there is another transfer for this pipe.  */
    current_packet =  asix -> ux_host_class_asix_xmit_queue;
    
    /* Get the next packet associated with the first packet.  */
    next_packet = current_packet -> nx_packet_queue_next;
    
    /* Set the next packet (or a NULL value) as the head of the xmit queue. */
    asix -> ux_host_class_asix_xmit_queue =  next_packet;
    
    /* If there is nothing else or if the link is down no need to rearm a packet. */
    if (next_packet != UX_NULL && asix -> ux_host_class_asix_link_state == UX_HOST_CLASS_ASIX_LINK_STATE_UP)
    {

        /* Load the address of the current packet header of the physical header.  */
        packet_header =  next_packet -> nx_packet_prepend_ptr;
    
        /* Substract 2 USHORT to store length of the packet.  */
        packet_header -= sizeof(USHORT) * 2;
    
        /* Store the length of the payload in the first USHORT.  */
        _ux_utility_short_put(packet_header, (USHORT)(next_packet -> nx_packet_length));
    
        /* Store the negative length of the payload in the first USHORT.  */
        _ux_utility_short_put(packet_header+ sizeof(USHORT), (USHORT)(~next_packet -> nx_packet_length));
    
        /* Prepare the values for this new transmission.  */
        transfer_request -> ux_transfer_request_data_pointer     =  packet_header;
        transfer_request -> ux_transfer_request_requested_length =  next_packet -> nx_packet_length + (ULONG)sizeof(USHORT) * 2;
        
        /* Store the packet that owns this transaction.  */
        transfer_request -> ux_transfer_request_user_specific = next_packet;

        /* Arm another transfer.  */
        _ux_host_stack_transfer_request(transfer_request);
    }        

    /* Free the packet that was just sent.  First do some housekeeping.  */
    current_packet -> nx_packet_prepend_ptr =  current_packet -> nx_packet_prepend_ptr + UX_HOST_CLASS_ASIX_ETHERNET_SIZE; 
    current_packet -> nx_packet_length =  current_packet -> nx_packet_length - UX_HOST_CLASS_ASIX_ETHERNET_SIZE;

    /* And ask Netx to release it.  */
    nx_packet_transmit_release(current_packet); 

    /* There is no status to be reported back to the stack.  */
    return; 
}

