package com.redatoms.chat.server.common.mq;

import com.redatoms.chat.server.common.data.ProtoMeta.DataPackage;

public abstract interface DataPackageListener {
	public abstract void onMessage(DataPackage dataPackage) throws Exception;
}