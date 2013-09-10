package com.redatoms.chat.server.common.dao;

import com.redatoms.chat.server.common.ebo.P2PMessageEbo;

public class P2PMessageDao extends BaseSortSetDao<P2PMessageEbo> {

	@Override
	public Class<P2PMessageEbo> getEboClass() {
		return P2PMessageEbo.class;
	}

}
