package com.redatoms.chat.server.common.event;

import java.io.Serializable;

import com.redatoms.chat.server.common.data.ProtoMeta.DataPackage;

public class MessageEvent implements Serializable {

	private static final long serialVersionUID = 1L;
	
	private DataPackage dataPackage;

	public DataPackage getDataPackage() {
		return dataPackage;
	}

	public void setDataPackage(DataPackage dataPackage) {
		this.dataPackage = dataPackage;
	}



}
