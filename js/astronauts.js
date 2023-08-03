(function() {
  'use strict';

  var rocket = { delay: 100 };
  
  // dispatch by url
  if (window.location.host == 'atcoder.jp') {
    rocket.fuel = () => {
      let opts = $('select#select-task')[0].options;
      return window.location.pathname.split('/')[2] + '/' +
        opts[opts.selectedIndex].value.split('_')[1];
    };
    rocket.launch = src => {
      $('.editor').data('editor').doc.setValue(src);
    };
    rocket.onchange = onchange => {
      $('select#select-task')[0].onchange = onchange;
    };
  }
  else if (window.location.host == 'www.codechef.com') {
    rocket.fuel = () => {
      let p = window.location.pathname.split('/');
      return p[1] + '/' + p[3];
    };
    rocket.first_delay = 8000;
    rocket.launch = src => {
      unsafeWindow.ace.edit('submit-ide-v2').setValue(src);
    };
    rocket.onchange = async onchange => {
      await setDelay(8000);
      $('h1[title]')[0].onclick = onchange;
    };
  }
  else {
    alert('Not support this CP yet');
    return;
  }

  const setDelay = (millisecond) => {
    return new Promise((resolve, reject)=>{
      if (typeof millisecond != 'number') reject(new Error('参数必须是number类型'));
      setTimeout(()=> {
        resolve(`我延迟了${millisecond}毫秒后输出的`)
      }, millisecond)
    })
  };

  function emit() {
    const task = new Promise((resolve, reject) => {
      GM.xmlHttpRequest({
        method: "GET",
        responseType: "arrayBuffer",
        url: 'file:///home/fool/Code/cf/' + rocket.fuel() + '/main.cc',
        headers: { "Content-Type": "text/x-c++src,charset=utf-8" },
        onload: data => resolve(new TextDecoder().decode(data.response)),
        onerror: error => reject(error)
      });
    })
      .then(async data => {
        await setDelay(rocket.delay);
        rocket.launch(data);
      })
      .catch(err => {
        console.log('rocket error', err);
      });
  }

  if (rocket.onchange != null) {
    rocket.onchange(emit);
  }
  (async () => {
    if (rocket.first_delay) {
      await setDelay(rocket.first_delay);
    }
    emit();
  })();
})();
