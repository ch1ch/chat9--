package com.redatoms.chat.server.common.ebo;

import com.redatoms.chat.server.common.data.ProtoMeta.DataPackage.Message;

public class P2PMessageEbo extends BaseSortSetEbo<Message> {

	private static final long serialVersionUID = 1L;
	
	public byte[] toByteArray() {
		return getAttributes().toByteArray();
	}

}
