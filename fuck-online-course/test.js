// ==UserScript==
// @name         Fuck Online Courses
// @namespace    http://tampermonkey.net/
// @version      1.0
// @description  摸鱼是第一生产力
// @author       Chikie
// @match        http://172-17-1-200-8080-p.webvpn.wbu.edu.cn:8118/integration/v2/*
// @require      https://cdn.jsdelivr.net/gh/colxi/getEventListeners/src/getEventListeners.min.js
// @grant        unsafeWindow
// @grant        GM_setValue
// @grant        GM_getValue
// @grant        GM_deleteValue
// @grant        GM_listValues
// ==/UserScript==


'use strict'

// Settings
let begin = 0
let counts;
let default_wait_time = 10;
let url_one = /courseOutlineStudent/i;
let url_two = /courseDetailStudent/i;
let url_three = /studyCenter/i;

function fuck_online_course() {
    let url = window.location.href;
    // 如果在视频列表则调用get_list
    if(url_one.test(url)){
        // alert('函数1被调用');
        let check_load = setInterval(() => {
            if(check_load_one()){
                clearInterval(check_load); // 关闭定时器
                get_list();
            }
        }, 2000);
    }

    // 如果在视频主页则调用wait_a_minute
    if(url_two.test(url)){
        let check_load = setInterval(() => {
            if(check_load_two()){
                clearInterval(check_load);
                // alert('调用waitaminute');
                wait_a_minute();
            }
        }, 2000);
    }

    if(url_three.test(url)){
        init();
    }
}

fuck_online_course();

function init() {
    GM_deleteValue('i');
    GM_deleteValue('counts');
}


function volume() {
    let target = document.getElementsByClassName('volume-bar')[0].style.width; // 获取音量部件style
    if(target != '0%'){
        // alert('volume执行了');
        document.getElementsByClassName('volume-button volume-icon')[0].click()
    }
    // 静音
}

function remove_judge() {
    // alert('remove执行了');
    //加个删除HTML的代码
    let item = document.getElementById('myModal3');
    item.parentNode.removeChild(item);
    // let target = console.log(window.document.getEventListeners()); // 获取监听事件
    // document.removeEventListener('visibilitychange', target);
    unsafeWindow.window.ins.pause = undefined;
    // 移除屏幕监控
}

function start_study() {
    let button = document.getElementsByClassName('btn btn-primary btn-sm handleBtn')[0];
    button.click();
    // 点击开始学习按钮
    let check_load = setInterval(() => {
        if(check_load_three() && document.getElementsByClassName('xdyplayer dplayer-loading').length == 0){
            clearInterval(check_load);
            unsafeWindow.goPaly();
            unsafeWindow.window.ins.play();
            remove_judge();
            volume();
            clearInterval(check_load);
            let remind_time = get_time();
            let back_to_list = setInterval(stay(), remind_time*1000);
            function stay() {
                if(document.getElementById('remainingTime').innerHTML == '00:01'){
                    document.getElementsByClassName('backLink pull-right')[0].click();
                    unsafeWindow.saveVideoRecord(0);
                    clearInterval(back_to_list);
                } else {
                    clearInterval(back_to_list);
                    back_to_list = setInterval(stay(), default_wait_time*1000);
                }
            }
        }
    }, 2000);
}

function get_time() {
    let time = document.getElementById('remainingTime').innerHTML;
    time = (Number(time[0])*10 + Number(time[1]))*60 + Number(time[3])*10 + Number(time[4]);
    return time;
}

let i = GM_getValue('i', begin);

function get_list() {
    // console.log('load');
    let video_lists = document.getElementsByClassName('panel-body')[0]; //视频列表
    counts = video_lists.childElementCount; // 获取视频总数
    video_lists.children[i++].children[1].click();
    GM_setValue('i', i);
    // 获取视频
}

function wait_a_minute() {
    if(document.getElementsByClassName('fyj_status_span').length == 1){ 
        // document.getElementsByClassName('fyj_status_span')[0].innerHTML == '已完成'
        // alert('1');
        document.getElementsByClassName('backLink pull-right')[0].click();
    } else {
        // 点击播放按钮
        // alert('2');
        start_study();
    }
}


function check_load_one() {
    // alert('1被调用');
    return (document.getElementsByClassName('fyj_loading text-center hide').length != 0 && document.getElementsByClassName('panel-body')[0] != undefined);
}

function check_load_two() {
    // alert('2被调用');
    return (document.getElementsByClassName('fyj_loading text-center hide').length != 0 && document.getElementsByClassName('btn btn-primary btn-sm handleBtn')[0] != undefined);
}

function check_load_three() {
    return document.getElementsByClassName('play-area clearfix hide').length == 0;
}