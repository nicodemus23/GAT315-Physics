#include "contact.h"

#include <stdlib.h>
#include <assert.h>
#include <string.h>

void AddContact(ncContact_t* contact, ncContact_t** contacts)
{
	assert(contact);

	// add element to linked list
	contact->next = *contacts;

	// set head of elements to new element
	*contacts = contact;
}

void DestroyAllContacts(ncContact_t** contacts)
{
	if (!*contacts) return;

	ncContact_t* contact = *contacts;
	while (contact)
	{
		ncContact_t* next = contact->next;
		free(contact);
		contact = next;
	}

	*contacts = NULL;
}