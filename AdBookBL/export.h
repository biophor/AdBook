#pragma once
#include <activeds.h>
#include <atlbase.h>
#include <set>
#include <map>
#include <vector>
#include <deque>
#include <list>
#include <memory>
#include <functional>

#include "AdBookBLExport.h"
#include "debug.h"
#include "error.h"
#include "shared.h"
#include "AdPersonDesc.h"
#include "Attributes.h"
#include "ConnectionParams.h"
#include "LdapRequestBuilder.h"
#include "AbstractAdAccessFactory.h"
#include "AbstractAdConnector.h"
#include "AbstractAdSearcher.h"
#include "AbstractAdPersonDescKeeper.h"

// for tests only.
#include "AdConnector.h"
#include "AdSearcher.h"
#include "AdPersonDescSqliteKeeper.h"
