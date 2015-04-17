#include "stdafx.h"
#include "ContactFinder.h"


ContactFinder::ContactFinder()
{
}


ContactFinder::~ContactFinder()
{
}

void ContactFinder::Start(const ConnectionSettings & cs)
{

}

void ContactFinder::Stop()
{
}

bool ContactFinder::IsSearching() const
{
    return false;
}

void ContactFinder::OnStopHandler(OnStop stopHandler)
{
}

void ContactFinder::OnNewContactFoundHandler(OnNewContact newContact)
{
}
