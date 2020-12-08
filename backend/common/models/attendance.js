'use strict';
var moment = require('moment');
module.exports = function(Attendance) {
    
    var getEmployeeWithRFID = async function(rfid){
        const { Employee } = Attendance.app.models
        var data = await Employee.find({where: {and:[{ rfid: rfid}]}, skip: 0, limit: 0})
        return Promise.resolve(data[0].__data);
    }

    var getAttendance = async function(dt, rd){
        console.log(dt)
        console.log(rd)
        var data = await Attendance.find({where: {date: dt, rfid: rd}})
        return Promise.resolve(data);
    }

    Attendance.postAttendance = (
        rfid,
        cb
    ) => {
        var newdate = new Date()

        getAttendance(moment(newdate).format("YYYY-MM-DD"), rfid).then(reg => {
            if(reg.length > 0){
                console.log("=========================================")
                console.log("rfid  " + rfid + "  is already registered")
                console.log("=========================================")
                cb(null, 4)
            }
            else {

                getEmployeeWithRFID(rfid).then(r => {

                    var startdate = new Date()
                    var tempdate = new Date()
                    startdate.setHours(2, 30, 25)
                
                    var tempobj = {
                        "date": moment(tempdate).format("YYYY-MM-DD"),
                        "value": "P",
                        "rfid": rfid,
                        "latemins": parseFloat((tempdate - startdate) / 1000/60/60).toFixed(2),
                        "employeeId": r.id
                      }
                    Attendance.create(tempobj)
                })

                cb(null, 2)
            }
        })
        
    }

    Attendance.remoteMethod("postAttendance", {
        description: "Post attedance",
        returns: {
            type: "number",
            root: true
        },
        accepts: {
            arg: "rfid",
            type: "string"
        },
        http: {
            verb: "post",
            path: "/postAttendance"
        }
    })
};
