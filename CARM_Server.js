const express = require('express');

const path = require('path');
const app = express();

//client
//const net = require("net");
//var socket = new net.Socket();

var SerialPort = require('serialport');
var serialPort = new SerialPort('COM20', false);

//var SerialPort2 = require('serialport');
//var serialPort2 = new SerialPort2('COM22', false);
//var SerialPort3 = require('serialport');`
//var serialPort3 = new SerialPort3('COM19', false);


var request = require('request');
var headers = {
    'User-Agent':       'Super Agent/0.0.1',
    'Content-Type':     'application/x-www-form-urlencoded'
}

// Configure the request


//client 종료

var bodyParser = require('body-parser');
var id = "wait";

var ADOXX_STATUS = "NoAccess"; // NoAccess, idle, wait, running, error, exit
var MACHINE_STATUS = "NoAccess"; // NoAccess, idle, wait, running, error, exit

var m = "";
var targetm = "";

var ADOXX_AUTOMOBILE_STATUS = "NoAccess"; // NoAccess, idle, wait, running, error, exit
var MACHINE_AUTOMOBILE_STATUS = "NoAccess"; // NoAccess, idle, wait, running, error, exit

var qs = "";
var exe_time = "";


var timeout = 1000;


serialPort.open(function () {
   console.log('connected...');
});

//serialPort2.open(function () {
   
//   console.log('connected2...');
//});

//serialPort3.open(function () {
//   console.log('connected3...');
//});no

app.listen(10000, () => {
  console.log('Express App on port 10000!');
  //console.log(path);
});

app.use(bodyParser.urlencoded({extended:false}));
app.use(bodyParser.text());

app.use(
function(req, res, next) {
   setTimeout((function () {
   console.log('Request URL:', req.originalUrl);
   console.log('Request Body:', req.body);
   console.log('req.query.id: ', req.query.id);
   console.log(req.body[1]);
  if (req.query.id == "adoxx")
  {
      console.log('online adoxx');
      var name = req.query.name
      
      console.log("name = " + name);
      if (ADOXX_STATUS == "NoAccess")
      {
         console.log("NoAccess");
         ADOXX_STATUS = "idle";
         res.send(ADOXX_STATUS);
      }
      else if (ADOXX_STATUS == "idle")
      {
         console.log("idle");
         // if (typeof(req.query.M1) != "undefined") { m = req.query.m1; targetm = "m1"; ADOXX_STATUS = "wait"; }
         // else if (typeof(req.query.M2) != "undefined") { m = req.query.m2; targetm = "m2"; ADOXX_STATUS = "wait"; }
         // else if (typeof(req.query.M3) != "undefined") { m = req.query.m3; targetm = "m3"; ADOXX_STATUS = "wait"; }
         // else if (typeof(req.query.M4) != "undefined") { m = req.query.m4; targetm = "m4"; ADOXX_STATUS = "wait"; }
         // else if (typeof(req.query.M5) != "undefined") { m = req.query.m5; targetm = "m5"; ADOXX_STATUS = "wait"; }
         // else if (typeof(req.query.M6) != "undefined") { m = req.query.m6; targetm = "m6"; ADOXX_STATUS = "wait"; }
         // else {}
   //      console.log(JSON
         res.send(ADOXX_STATUS);
      }      
      //Client
      if (name == "Automobile")
      {
         console.log(JSON.parse(req.body));
         command = "";
         var automobile01 = {
            url: 'http://192.168.1.3:10001',
            method: 'GET',
            headers: headers,
            body: req.body
            
         }
            //var SerialPort = require('serialport');
            //var serialPort = new SerialPort('COM4', false);
            
            //serialPort.open(function () {
            //console.log('connected...');
            //serialPort.write(req.body, function(err, results) {});
            //});
            
            //serialPort.close(function(err) {
            //   console.log('port closed', err);
            //});
            array = JSON.parse(req.body);
            for (var x in array)
            {
               //var command_index = 1;
               c = array[x];
               if(c[0] != '_') 
                  if( c[0] != '0' ) 
                     {
                        //command += (++command_index);
                        command+=c;
                     }
            }
            console.log(command);
            serialPort.write(command, function(err, results) {});
            console.log('t');         
      }
   }
   
   else if (req.query.id == "robotarm")
  {
      if (MACHINE_STATUS == "NoAccess")
      {

         res.send("@{"
                     +   "\"op\":\"idle\""
                     +   "}@");
         MACHINE_STATUS = "idle";
         
      }
      else if (MACHINE_STATUS == "idle")
      {
         if (ADOXX_STATUS == "wait")
         {

            res.send("@{"
                  +   "\"op\":\"running\","
//                  +   "\"index\":\"2\","
//                  +   "\"list\":[\"targetm\",\"degree\"]," //리스트 형식이 잘못된듯.. 수정 필요함
                  +   "\"targetm\":\""+targetm+"\","
                  +   "\"degree\":\""+m+"\""
                  +   "}@");

            MACHINE_STATUS = "running";
         }
         else
         {
            res.send("@{"
                     +   "\"op\":\"idle\""
                     +   "}@");
         }
      }
      else if (MACHINE_STATUS == "running")
      {
         // if (req.query.result == "complete")
         // {
         //      setTimeout(function() {
            
         //      }, 1000);

               res.send("@{"
                  +   "\"op\":\"idle\""
                  +   "}@");

               MACHINE_STATUS = "idle";
               ADOXX_STATUS = "idle";
               
         // }
         // else
         // {
            // MACHINE_STATUS = "error";
               // ADOXX_STATUS = "error";
               // res.send("@{"
                     // +   "\"op\":\"error\""
                     // +   "}");
         // }
      }
  }
  else if (req.query.id == "ado_automobile")
  {
      if (ADOXX_AUTOMOBILE_STATUS == "NoAccess")
      {
         ADOXX_AUTOMOBILE_STATUS = "idle";
         res.send(ADOXX_AUTOMOBILE_STATUS);
      }
      else if (ADOXX_AUTOMOBILE_STATUS == "idle")
      {
         if (typeof(req.query.QuickStart) != "undefined") { qs = req.query.QuickStart; ADOXX_AUTOMOBILE_STATUS = "wait"; }
         if (typeof(req.query.time) != "undefined") { exe_time = req.query.time; ADOXX_AUTOMOBILE_STATUS = "wait"; }

         res.send(ADOXX_AUTOMOBILE_STATUS);
      }
      else if (ADOXX_AUTOMOBILE_STATUS == "wait")
      {
         res.send(ADOXX_AUTOMOBILE_STATUS);
      }
  }
  else if (req.query.id == "mc_automobile")
  {
      if (MACHINE_AUTOMOBILE_STATUS == "NoAccess")
      {

         res.send("@{"
                     +   "\"op\":\"idle\""
                     +   "}@");
         MACHINE_AUTOMOBILE_STATUS = "idle";
      }
      else if (MACHINE_AUTOMOBILE_STATUS == "idle")
      {
         if (ADOXX_AUTOMOBILE_STATUS == "wait")
         {

            res.send("@{"
                  +   "\"op\":\"running\","
                  +   "\"QuickStart\":\""+qs+"\","
                  +   "\"time\":\""+exe_time+"\""
                  +   "}@");

            MACHINE_AUTOMOBILE_STATUS = "running";
         }
         else
         {
            res.send("@{"
                     +   "\"op\":\"idle\""
                     +   "}@");
         }
      }
      else if (MACHINE_AUTOMOBILE_STATUS == "running")
      {
               res.send("@{"
                  +   "\"op\":\"idle\""
                  +   "}@");

               MACHINE_AUTOMOBILE_STATUS = "idle";
               ADOXX_AUTOMOBILE_STATUS = "idle";
      }
  }
  console.log('end req');
  next();
}),timeout);
}

);
