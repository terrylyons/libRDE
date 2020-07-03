#pragma once

/// contains the ledger and 
/// connects the call back function from the trap to the ledger
namespace tjl
{
	template<typename ledger_t, trap* p_trap>
	class connect
	{
	public:
		ledger_t my_mirror;
		connect()
		{
			p_trap -> Ledger(&my_mirror);
		}
	};
}


