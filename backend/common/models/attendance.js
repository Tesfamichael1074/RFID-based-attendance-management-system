'use strict';
var moment = require('moment');
module.exports = function(Attendance) {
    
    var getEmployeeWithRFID = async function(rfid){
        const { Employee } = Attendance.app.models
        var data = await Employee.find({where: {and:[{ rfid: rfid}]}, skip: 0, limit: 0})
        return Promise.resolve(data[0].__data);
    }

    var getAttendance = async function(date){
        var data = await Attendance.find({where: {like: date}})
        return Promise.resolve(data);
    }

    Attendance.postAttendance = (
        rfid,
        cb
    ) => {
        var newdate = new Date()
        console.log(newdate)
        console.log(moment(newdate).format("YYYY-MM-DD"))


        // getAttendance().then(r => {
        //     console.log(r)
        // })
        
        // getEmployeeWithRFID(rfid).then(r => {
        //     var startdate = new Date()
        //     var tempdate = new Date()
        //     startdate.setHours(2, 30, 25)
           
        //     var tempobj = {
        //         "date": tempdate,
        //         "value": "P",
        //         "latemins": parseFloat((tempdate - startdate) / 1000/60/60).toFixed(2),
        //         "employeeId": r.id
        //       }
            
        //     // Attendance.create(tempobj)

        // })
        
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
