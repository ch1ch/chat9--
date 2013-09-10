package com.redatoms.chat.server.common.dao;

import com.redatoms.chat.server.common.ebo.BaseSortSetEbo;

public abstract class BaseSortSetDao<E extends BaseSortSetEbo> extends BaseDao<E> {

	@Override
	public E create(E ebo) throws Exception {
		return getEmgr().persistVersion(ebo);
	}
	
	
	@Override
	public E get(Long id) throws Exception {
		// TODO Auto-generated method stub
		return super.get(id);
	}
	
}
