#include "workmanager.h"

#include "eventreceiver.h"
#include "event.h"
#include "scopelock.h"

void WorkManager::init() {
  thread.start("Worker", this);
}

void WorkManager::dispatchFDData(EventReceiver * er, int sockid) {
  dataqueue.push(Event(er, WORK_DATA, sockid));
  event.post();
  readdata.wait();
}

void WorkManager::dispatchFDData(EventReceiver * er, int sockid, char * buf, int len) {
  dataqueue.push(Event(er, WORK_DATABUF, sockid, buf, len));
  event.post();
}

void WorkManager::dispatchTick(EventReceiver * er, int interval) {
  dataqueue.push(Event(er, WORK_TICK, interval));
  event.post();
}

void WorkManager::dispatchEventNew(EventReceiver * er, int sockid) {
  dataqueue.push(Event(er, WORK_NEW, sockid));
  event.post();
}

void WorkManager::dispatchEventConnected(EventReceiver * er, int sockid) {
  dataqueue.push(Event(er, WORK_CONNECTED, sockid));
  event.post();
}

void WorkManager::dispatchEventDisconnected(EventReceiver * er, int sockid) {
  dataqueue.push(Event(er, WORK_DISCONNECTED, sockid));
  event.post();
}

void WorkManager::dispatchEventSSLSuccess(EventReceiver * er, int sockid) {
  dataqueue.push(Event(er, WORK_SSL_SUCCESS, sockid));
  event.post();
}

void WorkManager::dispatchEventSSLFail(EventReceiver * er, int sockid) {
  dataqueue.push(Event(er, WORK_SSL_FAIL, sockid));
  event.post();
}

void WorkManager::dispatchEventFail(EventReceiver * er, int sockid, std::string error) {
  dataqueue.push(Event(er, WORK_FAIL, sockid, error));
  event.post();
}

void WorkManager::dispatchEventSendComplete(EventReceiver * er, int sockid) {
  dataqueue.push(Event(er, WORK_SEND_COMPLETE, sockid));
  event.post();
}

void WorkManager::dispatchSignal(EventReceiver * er, int signal) {
  if (signalevents.set(er, signal)) {
    event.post();
  }
}

void WorkManager::flushReceiver(EventReceiver * er) {
  ScopeLock lock(dataqueue.lock());
  bool erased = true;
  while (erased) {
    erased = false;
    for (std::list<Event>::iterator it = dataqueue.begin(); it != dataqueue.end(); it++) {
      if (it->getReceiver() == er) {
        dataqueue.erase(it);
        event.wait();
        erased = true;
        break;
      }
    }
  }
}

DataBlockPool * WorkManager::getBlockPool() {
  return &blockpool;
}

bool WorkManager::overload() const {
  return dataqueue.size() >= 10;
}

void WorkManager::run() {
  char * data;
  while(1) {
    event.wait();
    if (signalevents.hasEvent()) {
      SignalData signal = signalevents.getClearFirst();
      signal.er->signal(signal.signal);
    }
    else {
      Event event = dataqueue.pop();
      EventReceiver * er = event.getReceiver();
      int numdata = event.getNumericalData();
      switch (event.getType()) {
        case WORK_DATA:
          event.getReceiver()->FDData(numdata);
          readdata.post();
          break;
        case WORK_DATABUF:
          data = event.getData();
          er->FDData(numdata, data, event.getDataLen());
          blockpool.returnBlock(data);
          break;
        case WORK_TICK:
          er->tick(numdata);
          break;
        case WORK_CONNECTED:
          er->FDConnected(numdata);
          break;
        case WORK_DISCONNECTED:
          er->FDDisconnected(numdata);
          break;
        case WORK_SSL_SUCCESS:
          er->FDSSLSuccess(numdata);
          break;
        case WORK_SSL_FAIL:
          er->FDSSLFail(numdata);
          break;
        case WORK_NEW:
          er->FDNew(numdata);
          break;
        case WORK_FAIL:
          er->FDFail(numdata, event.getStrData());
          break;
        case WORK_SEND_COMPLETE:
          er->FDSendComplete(numdata);
          break;
      }
    }
  }
}
