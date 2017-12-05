#include "AuthSample.h"


void supernode::AuthSample::Init()  {
	m_DAPIServer->ADD_DAPI_HANDLER(PosProxySale, rpc_command::RTA_TRANSACTION_OBJECT, AuthSample);
	m_DAPIServer->ADD_DAPI_HANDLER(WalletProxyPay, rpc_command::RTA_TRANSACTION_OBJECT, AuthSample);
}


bool supernode::AuthSample::PosProxySale(const rpc_command::RTA_TRANSACTION_OBJECT::request& in, rpc_command::RTA_TRANSACTION_OBJECT::response& out) {
//	out.Fail = true;

	RTA_TransactionRecord tr;
	if( !rpc_command::ConvertToTR(tr, in, m_Servant) ) return false;
	if( !Check(tr) ) return false;

	boost::shared_ptr<AuthSampleObject> data = boost::shared_ptr<AuthSampleObject>( new AuthSampleObject() );
	Setup(data);
	if( !data->Init(tr) ) return false;

	data->PosIP = in.SenderIP;
	data->PosPort = in.SenderPort;
	data->PayToWallet = in.PayToWallet;

	Add(data);


//	out.Fail = false;
	return true;
}

bool supernode::AuthSample::WalletProxyPay(const rpc_command::RTA_TRANSACTION_OBJECT::request& in, rpc_command::RTA_TRANSACTION_OBJECT::response& out) {
//	out.Fail = true;

	boost::shared_ptr<BaseRTAObject> ff = ObjectByPayment(in.PaymentID);
	boost::shared_ptr<AuthSampleObject> data = boost::dynamic_pointer_cast<AuthSampleObject>(ff);
	if(!data) return false;

	if(data->PayToWallet!=in.PayToWallet) { RemoveRecord(data); return false; }

	data->WalletIP = in.SenderIP;
	data->WalletPort = in.SenderPort;

	RTA_TransactionRecord tr;
	if( !rpc_command::ConvertToTR(tr, in, m_Servant) ) { RemoveRecord(data); return false; }

	if( !data->WalletProxyPay(tr) ) { RemoveRecord(data); return false; }


//	out.Fail = false;
	return true;
}


bool supernode::AuthSample::Check(RTA_TransactionRecord& tr) {
	// TODO: IMPL check sample selection if ok for givved block number
	return true;
}



void supernode::AuthSample::RemoveRecord(boost::shared_ptr<AuthSampleObject> record) {}



void supernode::rpc_command::ConvertFromTR(RTA_TRANSACTION_OBJECT::request& dst, const RTA_TransactionRecord& src) {}

bool supernode::rpc_command::ConvertToTR(RTA_TransactionRecord& dst, const RTA_TRANSACTION_OBJECT::request& src, const FSN_ServantBase* servant) {
	return true;
}







