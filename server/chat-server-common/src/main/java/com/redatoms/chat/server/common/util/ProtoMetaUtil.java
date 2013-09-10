package com.redatoms.chat.server.common.util;

import com.redatoms.chat.server.common.data.ProtoMeta;
import com.redatoms.chat.server.common.data.ProtoMeta.DataPackage;
import com.redatoms.chat.server.common.data.ProtoMeta.DataPackage.CmdType;
import com.redatoms.chat.server.common.data.ProtoMeta.DataPackage.Data;

public class ProtoMetaUtil {

	public static String print(ProtoMeta.DataPackage dataPackage) {
		return "DataPackage:\n" + dataPackage;
	}
	
	public static DataPackage buildResponsePackage(DataPackage requestPackage, CmdType cmd, Data data) {
		DataPackage responsePackage = DataPackage.newBuilder().setSeqNo(requestPackage.getSeqNo()).setVersion(requestPackage.getVersion()).setCmd(cmd).setData(data).build();
		return responsePackage;
	}
}
