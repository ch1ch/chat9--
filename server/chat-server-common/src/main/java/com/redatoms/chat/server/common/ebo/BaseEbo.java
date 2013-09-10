package com.redatoms.chat.server.common.ebo;

import java.io.Serializable;
import java.util.Calendar;
import java.util.Date;

import org.apache.log4j.Logger;

public abstract class BaseEbo<T> implements Serializable {

	private static final long serialVersionUID = 1L;
	
	private static Logger logger = Logger.getLogger(BaseEbo.class);
	
	private long id;
	
	private T attributes;
	
	private Date createTime;
	
	public abstract byte[] toByteArray();
	
	public void postGet() throws Exception {
		
	}
	
	public void preCreate() throws Exception {
		logger.debug("entering preCreate");
		createTime = Calendar.getInstance().getTime();
	}
	
	public void postCreate() throws Exception {
		
	}
	
	public void preUpdate() throws Exception {
		
	}
	
	public void postUpdate() throws Exception {
		
	}
	
	public void preDelete() throws Exception {
		
	}
	
	public void postDelete() throws Exception {
		
	}

	public Date getCreateTime() {
		return createTime;
	}

	public long getId() {
		return id;
	}

	public void setId(long id) {
		this.id = id;
	}

	public T getAttributes() {
		return attributes;
	}

	public void setAttributes(T attributes) {
		this.attributes = attributes;
	}
}
