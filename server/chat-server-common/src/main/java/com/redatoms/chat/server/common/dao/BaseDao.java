package com.redatoms.chat.server.common.dao;

import java.util.List;

import org.apache.log4j.Logger;

import com.redatoms.chat.server.common.ebo.BaseEbo;
import com.redatoms.chat.server.common.util.EntityManager;

public abstract class BaseDao<E extends BaseEbo> {

	private static final Logger log = Logger.getLogger(BaseDao.class);

	private EntityManager emgr;

	public BaseDao() {

	}

	public BaseDao(EntityManager emgr) {
		this.emgr = emgr;
	}

	public EntityManager getEmgr() throws Exception {
		if (emgr == null) {
			emgr = EntityManager.getInstance();
		}
		return emgr;
	}

	public abstract Class<E> getEboClass();
	
	public E get(Long id) throws Exception {
		E ebo = getEmgr().find(getEboClass(), id);
		ebo.postGet();
		return ebo;
	}

	public List<E> list() throws Exception {
		List<E> list = getEmgr().findAll(getEboClass());
		return null;
	}


	public E create(E ebo) throws Exception {
		getEmgr().persist(ebo);
		getEmgr().flush();
		return ebo;
	}
}
