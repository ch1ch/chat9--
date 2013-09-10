package com.redatoms.chat.server.common.listener;

import java.util.List;

import com.redatoms.chat.server.common.data.ProtoMeta.DataPackage;

public interface BatchHandlerListener {

	public void onBatchHandle(List<DataPackage> list);
}
